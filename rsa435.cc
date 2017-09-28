//Anthony Scaglione, University of Akron
//ams390@zips.uakron.edu
//september 27th, 2017
//
//This program can generate encryption keys for RSA435, and use them to
//sign and verify documents

// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <streambuf>
#include <sstream>

//sha256 library
#include "sha256.h"

// `BigIntegerLibrary.hh' includes all of the library headers.
#include "BigIntegerLibrary.hh"

//functons implemented
BigInteger genBigPrime();
bool fTest(BigInteger);
BigInteger modulo(BigInteger, BigInteger, BigInteger);
BigInteger exEuc(BigInteger ,BigInteger);
void keyGen(BigInteger, BigInteger);
void sign(std::string);
void verify(std::string);
BigInteger modInv(BigInteger, BigInteger, BigInteger &, BigInteger &);



int main(int argc, char** argv) {
	/* The library throws `const char *' error messages when things go
	 * wrong.  It's a good idea to catch them using a `try' block like this
	 * one.  Your C++ compiler might need a command-line option to compile
	 * code that uses exceptions. */
	try {
		std::string arg = argv[1];
		if(arg == "k"){
			std::cout << "Generating Keys..." << std::endl;
			BigInteger p = BigInteger(1);
			BigInteger q = BigInteger(1);
			keyGen(p,q);
		}else if(arg == "s"){
			sign(argv[2]);
		}else if(arg == "v"){
			verify(argv[2]);
		}else{
			std::cout << "Incorrect arguments: Use k for keygen, s to sign, v to verify" << std::endl;
		}
	} catch(char const* err) {
		std::cout << "The library threw an exception:\n"
			<< err << std::endl;
	}

	return 0;
}

//verifies the file based on m^e%n=c
//verification occurs when the encryption of the saved hash code matches
//the encoded content part of the file
void verify(std::string fileName){
	//fetch decrypted signature from file
	std::ifstream ifs(fileName.c_str());
	std::string decrypted;
	while (ifs >> std::ws && std::getline(ifs, decrypted));
	ifs.close();
	//fetch content from file
	ifs.open(fileName.c_str());
	std::string input((std::istreambuf_iterator<char>(ifs)),(std::istreambuf_iterator<char>()));
	std::string::size_type i = input.find(decrypted);
	if (i != std::string::npos)
  	input.erase(i, decrypted.length());
	//if file isnt a txt file, place saved hash in the right place
	if (!(fileName.find(".txt") != std::string::npos)) {
	  input.resize (input.size () - 1);
	}
	//generate hash of content
	std::string shaOut = sha256(input);
	//fetch keys from keyfile
	BigUnsigned content = BigUnsignedInABase(shaOut, 16);
	ifs.close();
	ifs.open("e_n.txt");
	std::string eIn = "";
	std::getline(ifs,eIn); //get e
	BigInteger e = stringToBigInteger(eIn);
	std::string nIn = "";
	std::getline(ifs,nIn); //get n
	BigInteger n = stringToBigInteger(nIn);
	BigInteger m = stringToBigInteger(decrypted);

	//test verification
	if(modulo(m, e, n) == content){
		std::cout << "The file has been verified" << std::endl;
	}else{
		std::cout << "The file has failed verification" << std::endl;
	}


}

//signs a file, given the filename
//saves the resulting content and hash code as a new file
//listed as [filename].signed
void sign(std::string fileName){
	//fetch data from text files
	std::ifstream ifs(fileName.c_str(), std::ios::in | std::ios::binary);
	std::ostringstream oss;
	oss << ifs.rdbuf();
	std::string data(oss.str());
	//std::string input((std::istreambuf_iterator<char>(ifs)),(std::istreambuf_iterator<char>()));
	ifs.close();
	std::string shaOut = sha256(data);
	//fetch keys from keyfile
	BigUnsigned c = BigUnsignedInABase(shaOut, 16);
	ifs.open("d_n.txt");
	std::string dIn = "";
	getline(ifs,dIn); //get d
	BigInteger d = stringToBigInteger(dIn);
	std::string nIn = "";
	getline(ifs,nIn); //get n
	BigInteger n = stringToBigInteger(nIn);
	//return signed value
	fileName = fileName + ".signed";
	std::ofstream ofs(fileName.c_str(), std::ios::out | std::ios::trunc);
	ofs << data;
	if (!(fileName.find(".txt") != std::string::npos)) {
    ofs << std::endl;
	}
	ofs << modulo(c, d, n);
	ofs.close();
}

//Function generates the keys required for encryption.
//e is constant, while d is computed
//both saved in e_n.txt and d_n.txt respectively
void keyGen(BigInteger p, BigInteger q){
	p = genBigPrime();
	q = genBigPrime();
	BigInteger n = p*q;
	BigInteger phi = (p-1)*(q-1);
	BigInteger e = 65537;
	BigInteger x, y;
	BigInteger d = modInv(phi, e, x, y);
	if(d < 0)
		d += phi;
	BigInteger test = (e*d)%phi;
	//write the found keys to a file
	std::ofstream myfile;
	myfile.open("e_n.txt");
	myfile << e << "\n" << n;
	myfile.close();
	myfile.open("d_n.txt");
	myfile << d << "\n" << n;
	myfile.close();
}

//generates large prime numbers
BigInteger genBigPrime(){
	BigInteger big1 = BigInteger(1);
	do{
		big1 = 1;
		for (int i=0;i<400;i++) { //TODO -switch back to 400
			 big1 = big1*10 +rand();
		}
	}while(!fTest(big1));
	return big1;
}

//fermats little theorem
//test for primality
bool fTest(BigInteger p){
	for (int i = 2; i < 5; i++){
			if (modulo(i, p-1, p) != 1){
					return false;
			}
		}
		return true;
}

//calculates and returns x^y%m
BigInteger modulo(BigInteger x, BigInteger y, BigInteger m){
	if(y == 0){
		return 1;
	}
	BigInteger z = modulo(x, y/2, m);
	if(y%2 == 0){
		return (z*z)%m;
	}else{
		return (z*z*x)%m;
	}
}

//extended euclidean
//finds modular inverse
//stackoverflow is a lifesaver
BigInteger modInv(BigInteger a, BigInteger b, BigInteger &x, BigInteger &y) {
    if(b == 0) {
       x = 1;
       y = 0;
       return a;
    }
    BigInteger x1, y1, gcd = modInv(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return y; //y is what returns as the value of d
}
