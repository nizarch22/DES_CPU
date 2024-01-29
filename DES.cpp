#include <iostream>
#include "DES.h"
#include "DES_Matrices_NIST.h"


// global variables
uint64_t key;
uint64_t plaintext;

// input 6: divided into V[1:0]={in5,in0} - rows, A[3:0]={in4,in3,in2} - columns. 
// output 4 bits
void EncryptDES()
{
	
	generateKey();
	initialPermutation();
	// result = input >> 32; // i.e. Result[31:0] = Input[63:32]
	for (int i = 0; i < 16; i++)
	{
		generateRoundKey();
		expandPermutation(); // result1
		// result2 = result1^roundKey;
		substitute(); // result3
		mixPermutation(); // result 4
		// xxx
	}
	// input <<=32; input>>=32; // i.e. making input[63:32] = 0
	//result += input // i.e. Result[63:32] = Input[31:0] ^ Result4[31:0]
	
	swapLR();
	reverseInitialPermutation();
}

void DecryptDES()
{

}

// auxiliary functions

void swapLR()
{

}


void initialPermutation()
{

}

void reverseInitialPermutation()
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

void substitute()
{
	// 
}


void mixPermutation()
{

}

// Matrix helper functions
void permuteMatrix(uint64_t* input, unsigned int* P, unsigned int size)
{
	uint64_t output = 0;
	uint64_t bit;

	for (int i = 0; i < size; i++)
	{
		bit = (*input >> (P[i] - 1)) & 1;
		output += bit << i;
	}
	*input = output;
}

// Debug functions
void printMatrix(uint64_t matrix, int y, int x)
{
	bool bit;
	bool mask = 1;
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{

			bit = matrix&mask;
			std::cout << bit << ",";
			matrix >>= 1;
		}
		std::cout << "\n";
	}
	std::cout << "Matrix printed.\n";
}

int bEqualMatrix(const uint64_t& m1, const uint64_t& m2, int size)
{
	bool bit;
	uint64_t mask = 1;
	for (int i = 0; i < size; i++)
	{
		bit = (m1 & mask) == (m2 & mask);
		mask <<= 1;

		if (bit == 0)
			return 0;
	}
	return 1;
}









