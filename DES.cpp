#include <iostream>
#include <random>
#include "DES.h"
#include "DES_Matrices_NIST.h"

// input 6: divided into V[1:0]={in5,in0} - rows, A[3:0]={in4,in3,in2} - columns. 
// output 4 bits

// auxiliary functions

void initialPermutation(uint64_t& input)
{
	permuteMatrix(input, IP, 64);
}


void generateKey(uint64_t& key)
{
	// 64 bits
	key = ((uint64_t)rand()) << 32 | rand();
	permuteMatrix(key, PC1, 56);
}

void leftCircularShift(uint32_t& input, uint8_t times)
{
	uint32_t mask28thBit = 1 << 27; // 28th bit
	uint32_t mask28Bits = 268435455; // covers first 28 bits

	uint8_t bit;
	for (int i = 0; i < times; i++)
	{
		bit = (input & mask28thBit)>>27;
		input <<= 1;
		input += bit;
	}
	input = input & mask28Bits;

}
void generateRoundKey(const int& index, uint64_t& roundKey)
{
	uint32_t left, right;
	uint64_t mask28Bits = 268435455; // covers first 28 bits

	right = roundKey & mask28Bits;
	mask28Bits <<= 28;
	mask28Bits = roundKey & mask28Bits;
	left = mask28Bits >> 28;
	leftCircularShift(left, LCS[index]);
	leftCircularShift(right, LCS[index]);

	// copying left and right shifted keys to roundKey.
	roundKey = left;
	roundKey <<= 28;
	roundKey += right;
}

void roundKeyPermutation(uint64_t& roundKey)
{
	permuteMatrix(roundKey, PC2, 48);
}

void expandPermutation(uint64_t& input)
{
	permuteMatrix(input, E, 48);
}

void substitute(uint64_t& input)
{
	uint64_t result = 0; uint64_t temp;
	uint8_t y, x;
	uint8_t in;

	uint64_t mask = 63;
	uint8_t maskY1, maskY2, maskX;
	maskY1 = 1;
	maskY2 = 32;
	maskX = 30;
	for (int i = 0; i < 8; i++)
	{
		// getting x,y coordinates for Sbox
		in = input & mask;
		x = (in & maskX)>>1;
		y = (in & maskY2)>>4;
		y += in & maskY1;

		// Substitution 
		temp = SBoxes[i][y * 16 + x];
		result += temp << (4*i);

		// next bits
		input >>= 6;
	}
	input = result;
}


void mixPermutation(uint64_t& input)
{
	permuteMatrix(input, PMatrix, 32);
}


void reverseInitialPermutation(uint64_t& input)
{
	permuteMatrix(input, IPInverse, 64);
}


void swapLR(uint64_t& input)
{
	uint64_t temp = input;
	// containing left side 
	temp >>= 32;

	// right side moved to left
	input <<= 32;

	// left side moved to right
	input += temp;
}


// Matrix helper functions
void permuteMatrix(uint64_t& input, const unsigned char* P, const unsigned int size)
{
	uint64_t output = 0;
	uint64_t bit;

	for (int i = 0; i < size; i++)
	{
		bit = (input >> (P[i] - 1)) & 1;
		output += bit << i;
	}
	input = output;
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

			bit = matrix & mask;
			std::cout << bit << ",";
			matrix >>= 1;
		}
		std::cout << "\n";
	}
	std::cout << "Matrix printed.\n";
}

int bEqualMatrix(const uint64_t& m1, const uint64_t& m2, const int size)
{
	//bool bit;
	//uint64_t mask = 1;
	//for (int i = 0; i < size; i++)
	//{
	//	bit = ((m1 & mask) == (m2 & mask));
	//	mask <<= 1;

	//	if (bit == 0)
	//		return 0;
	//}
	//return 1;
	return m2 == m1;
}

void initDES(uint64_t& key)
{
	generateKey(key);
}

void EncryptDES(const uint64_t& plaintext, const uint64_t& key, uint64_t& result, uint64_t keys[16])
{
	uint64_t input = plaintext;
	uint64_t roundKey = key;
	uint64_t permutedRoundKey;
	
	initialPermutation(input);
	unsigned int left = 0;
	
	for (int i = 0; i < 16; i++)
	{
		// Result[63:32] = Input[31:0];
		result = input;
		result <<= 32;
		// preserve left side
		left = input >> 32;

		generateRoundKey(i, roundKey);

		//
		keys[i] = roundKey; // remove later
		//
		permutedRoundKey = roundKey;
		roundKeyPermutation(permutedRoundKey);

		expandPermutation(input); // 48 bits
		input ^= permutedRoundKey;
		substitute(input); // 32 bits
		mixPermutation(input); 
		result += left^input; // Result[31:0] = L XOR f[31:0];

		input = result;
	}
	
	swapLR(result);
	reverseInitialPermutation(result);
}

void DecryptDES(const uint64_t& encryption, const uint64_t keys[16], uint64_t& decryption)
{
	uint64_t input = encryption;
	uint64_t roundKey, permutedRoundKey;
	uint64_t& result = decryption;

	initialPermutation(input);
	unsigned int left = 0;

	for (int i = 0; i < 16; i++)
	{
		// Result[63:32] = Input[31:0];
		result = input;
		result <<= 32;
		// preserve left side
		left = input >> 32;
		//
		roundKey = keys[15 - i]; // remove
		//
		permutedRoundKey = roundKey;
		roundKeyPermutation(permutedRoundKey);

		expandPermutation(input); // 48 bits
		input ^= permutedRoundKey;
		substitute(input); // 32 bits
		mixPermutation(input);
		result += left ^ input; // Result[31:0] = L XOR f[31:0];

		input = result;
	}

	swapLR(result);
	reverseInitialPermutation(result);
}

void foo()
{
	uint64_t keys[16] = {9999};
	uint64_t key;
	uint64_t plaintext = rand() | rand();
	uint64_t result;
	printMatrix(plaintext, 8, 8);
	initDES(key);
	EncryptDES(plaintext,key, result,keys);
	//for (int i = 0; i < 16; i++)
	//	std::cout << "Key " << i << ": " << keys[i] << "\n";
	uint64_t decryption;
	DecryptDES(result, keys, decryption);
	printMatrix(decryption, 8, 8);
	
	std::cout << "Was decryption successful? " << (bEqualMatrix(plaintext, decryption, 64) ? "true":"false") << "\n";
	// LCS test
	//uint32_t test = 1 + 2 + 4 + 8;
	//test += 1 << 25;
	//test += 1 << 26;
	//printMatrix(test, 1, 28);
	//leftCircularShift(test, 1);
	//printMatrix(test, 1, 28);
	//leftCircularShift(test, 1);
	//printMatrix(test, 1, 28);
	//leftCircularShift(test, 1);
	//printMatrix(test, 1, 28);
	//leftCircularShift(test, 1);
	//printMatrix(test, 1, 28);
}



