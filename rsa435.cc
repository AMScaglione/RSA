//Anthony Scaglione, University of Akron

// You need to complete this program for a part of your first project.

// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>


// `BigIntegerLibrary.hh' includes all of the library headers.
#include "BigIntegerLibrary.hh"

BigInteger genBigPrime();
bool fTest(BigInteger);
BigInteger modulo(BigInteger, BigInteger, BigInteger);
BigInteger exEuc(BigInteger ,BigInteger);

int main() {
	/* The library throws `const char *' error messages when things go
	 * wrong.  It's a good idea to catch them using a `try' block like this
	 * one.  Your C++ compiler might need a command-line option to compile
	 * code that uses exceptions. */
	try {

      std::cout << "a couple of test cases for 3460:435/535 Algorithms!!!\n";
      BigInteger p = BigInteger(1);
			p = genBigPrime();
			std::cout << "\nmy p !!!\n";
      std::cout << p;
      BigInteger q = BigInteger(1);
      q = genBigPrime();
      std::cout << "\nmy q !!!\n";
      std::cout << q;
      std::cout << "\nmy n = p*q !!!\n";
      BigInteger n = p*q;
      std::cout << n;
      //std::cout << "my n/q !!!\n";
      //std::cout << n/q;
			BigInteger phi = (p-1)*(q-1);
			std::cout << "\nmy phi = (p-1)*(q-1) !!!\n";
			std::cout << phi;
			BigInteger e = 65537;
			BigInteger d = exEuc(phi, e);

			std::ofstream myfile;
			myfile.open("e_n.txt");
			myfile << e << "\n" << n;
			myfile.close();
			myfile.open("d_n.txt");
			myfile << d << "\n" << n;
			myfile.close();



	} catch(char const* err) {
		std::cout << "The library threw an exception:\n"
			<< err << std::endl;
	}

	return 0;
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
	BigInteger temp, quot;
		while(b != 0){
			quot = a / b;
			temp = b;
			b = a % b;
			a = temp;
			temp = x;
			x = prevX-quot*x;
			prevX = temp;
			temp = y;
			y = prevY-quot*y;
			prevY = temp;
	  }
	  return prevY; //this ends up being d
}

//generates large prime numbers
BigInteger genBigPrime(){
	BigInteger big1 = BigInteger(1);
	do{
		big1 = 1;
		for (int i=0;i<200;i++) { //TODO -switch back to 400
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

//Credit to Khan Academy for helping with this one
//modular exponent function
/*
BigInteger modulo(BigInteger base, BigInteger mod){
		BigInteger exponent = mod - 1;
    BigInteger x = 1;
    BigInteger y = base;
    while (exponent > 0){
        if (exponent % 2 == 1)
            x = (x * y) % mod;
        y = (y * y) % mod;
        exponent = exponent / 2;
    }
    return x % mod;
}
*/
