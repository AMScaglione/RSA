//Anthony Scaglione, University of Akron

// You need to complete this program for a part of your first project.

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

BigInteger genBigPrime();
bool fTest(BigInteger);
BigInteger modulo(BigInteger, BigInteger, BigInteger);
BigInteger exEuc(BigInteger ,BigInteger);
void keyGen(BigInteger, BigInteger);
void sign(std::string);
void verify(std::string);

int main() {
	/* The library throws `const char *' error messages when things go
	 * wrong.  It's a good idea to catch them using a `try' block like this
	 * one.  Your C++ compiler might need a command-line option to compile
	 * code that uses exceptions. */
	try {
			BigInteger p = BigInteger(1);
			BigInteger q = BigInteger(1);
			//keyGen(p,q);
			sign("file.txt");
			verify("file.txt.signed");

			//BigInteger outputInt = stringToBigInteger(output);

			//std::cout << sign("file.txt") << std::endl;


	} catch(char const* err) {
		std::cout << "The library threw an exception:\n"
			<< err << std::endl;
	}

	return 0;
}
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
	//generate hash of content
	std::string shaOut = sha256(input);
	//fetch keys from keyfile
	BigUnsigned content = BigUnsignedInABase(shaOut, 16);
	std::cout << content << std::endl;
	ifs.open("e_n.txt");
	std::string eIn = "";
	getline(ifs,eIn); //get e
	BigInteger e = stringToBigInteger(eIn);
	std::string nIn = "";
	getline(ifs,nIn); //get n
	BigInteger n = stringToBigInteger(nIn);

	std::cout << modulo(stringToBigInteger(decrypted), e, n) << std::endl;

	BigInteger m = stringToBigInteger(decrypted);

	//test verification
	if(modulo(m, e, n) == content){
		std::cout << "verified" << std::endl;
	}else{
		std::cout << "fail" << std::endl;
	}


}

//signs a file, given the filename
void sign(std::string fileName){
	//fetch data from text files
	std::ifstream ifs(fileName.c_str());
	std::string input((std::istreambuf_iterator<char>(ifs)),(std::istreambuf_iterator<char>()));
	ifs.close();
	std::string shaOut = sha256(input);
	//fetch keys from keyfile
	BigUnsigned c = BigUnsignedInABase(shaOut, 16);
	std::cout << c << std::endl;
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
	ofs << input;
	ofs << modulo(c, d, n);
	ofs.close();
}

void keyGen(BigInteger p, BigInteger q){
	p = genBigPrime();
	q = genBigPrime();
	BigInteger n = p*q;
	BigInteger phi = (p-1)*(q-1);
	BigInteger e = 65537;
	BigInteger d = exEuc(phi, e);
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

//extended euclidean algorithm
//really sketchy but functional implimentation of extended euclidean
//as explained in this video https://www.youtube.com/watch?v=Z8M2BTscoD4&t=704s
//
//returns the value of d
BigInteger exEuc(BigInteger a, BigInteger b){
	BigInteger x = 0;
	BigInteger prevX = 1; //holds the previous value of x
	BigInteger y = 1;
	BigInteger prevY = 0; //holds the previous value of y
	BigInteger temp, q;
		while(b != 0){
			q = a / b;
			temp = b;
			b = a % b;
			a = temp;
			temp = x;
			x = prevX-q*x;
			prevX = temp;
			temp = y;
			y = prevY-q*y;
			prevY = temp;
	  }
	  return prevY; //this ends up being d
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
