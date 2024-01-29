#include <iostream>
#include "DES.h"

// input 6: divided into V[1:0]={in5,in0} - rows, A[3:0]={in4,in3,in2} - columns. 
// output 4 bits
unsigned int SBOX[8][4][16];
uint64_t key;
uint64_t plaintext;

void encryptDES()
{
	
	generateKey();
	initialPermutation();
	// result = input >> 32; // i.e. Result[31:0] = Input[63:32]
	for (int i = 0; i < 16; i++)
	{
		generateRoundKey();
		expandPermutation(); // result1
		// result2 = result1^roundKey;
		Substitute(); // result3
		Transpose(); // result 4
		// xxx
	}
	// input <<=32; input>>=32; // i.e. making input[63:32] = 0
	//result += input // i.e. Result[63:32] = Input[31:0] ^ Result4[31:0]
}

void decryptDES()
{

}


void initialPermutation()
{

}



void generateKey()
{
	// 64 bits
}


void generateRoundKey()
{
}

void expandPermutation()
{

}

void Substitute()
{

}


void Transpose()
{

}

// debug functions

void printMatrix(int y, int x, uint64_t matrix)
{
	unsigned char bit;
	uint64_t mask = 1;
	unsigned int a, b;
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			bit = matrix&mask;
			std::cout << bit << ",";
			mask <<= 1;
		}
		std::cout << "\n";
	}
}










