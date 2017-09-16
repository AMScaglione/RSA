// You need to complete this program for a part of your first project.

// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h>
#include <math.h>


// `BigIntegerLibrary.hh' includes all of the library headers.
#include "BigIntegerLibrary.hh"

BigInteger genBigPrime();
bool fTest(BigInteger);
BigInteger modulo(BigInteger, BigInteger, BigInteger);

int main() {
	/* The library throws `const char *' error messages when things go
	 * wrong.  It's a good idea to catch them using a `try' block like this
	 * one.  Your C++ compiler might need a command-line option to compile
	 * code that uses exceptions. */
	try {

      std::cout << "a couple of test cases for 3460:435/535 Algorithms!!!\n";
      BigInteger big1 = BigInteger(1);
			big1 = genBigPrime();
			std::cout << "my big1 !!!\n";
      std::cout << big1;
      BigInteger big2 = BigInteger(1);
      big2 = genBigPrime();
      std::cout << "my big2 !!!\n";
      std::cout << big2;
      std::cout << "my big3 = big1*big2 !!!\n";
      BigInteger big3 = big1*big2;
      std::cout <<big3;
      std::cout << "my big3/big2 !!!\n";
      std::cout <<big3/big2;

	} catch(char const* err) {
		std::cout << "The library threw an exception:\n"
			<< err << std::endl;
	}

	return 0;
}

BigInteger genBigPrime(){
	BigInteger big1 = BigInteger(1);
	do{
		big1 = 1;
		for (int i=0;i<400;i++) {
			 big1 = big1*10 +rand();
		}
	}while(!fTest(big1));
	return big1;
}

bool fTest(BigInteger p){
	if (p == 1){
		return false;
	}
	for (int i = 2; i < 6; i++){
			if (modulo(i, p - 1, p) != 1){
					return false;
			}
		}
		return true;
}

BigInteger modulo(BigInteger base, BigInteger exponent, BigInteger mod){
    BigInteger x = 1;
    BigInteger y = base;
    while (exponent > 0)
    {
        if (exponent % 2 == 1)
            x = (x * y) % mod;
        y = (y * y) % mod;
        exponent = exponent / 2;
    }
    return x % mod;
}
