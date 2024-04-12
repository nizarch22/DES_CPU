#include <iostream>
#include <chrono>
#include <random>
#include <time.h>
#include "DES.h"
#include "DES_Matrices_NIST.h"
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
void rightCircularShift(uint32_t& input, uint8_t times)
{
	uint32_t bit28th = 1 << 27; // 28th bit
	uint32_t mask1stBit = 1; // 28th bit
	uint32_t mask28Bits = 268435455; // covers first 28 bits

	uint32_t bit;
	for (int i = 0; i < times; i++)
	{
		bit = (input & mask1stBit);
		input >>= 1;
		input += bit * bit28th;
	}
	input = input & mask28Bits;

}
void generateRoundKey(const int& index, uint64_t& roundKey)
{
	uint32_t left, right;
	uint64_t mask28Bits = 268435455; // covers first 28 bits

	// getting left and right sides
	right = roundKey & mask28Bits;
	mask28Bits <<= 28;
	mask28Bits = roundKey & mask28Bits;
	left = mask28Bits >> 28;

	// circular shifts
	leftCircularShift(left, LCS[index]);
	leftCircularShift(right, LCS[index]);

	// copying left and right shifted keys to roundKey.
	roundKey = left;
	roundKey <<= 28;
	roundKey += right;
}
void generateReverseRoundKey(const int& index, uint64_t& roundKey)
{
	uint32_t left, right;
	uint64_t mask28Bits = 268435455; // covers first 28 bits

	// getting left and right sides
	right = roundKey & mask28Bits;
	mask28Bits <<= 28;
	mask28Bits = roundKey & mask28Bits;
	left = mask28Bits >> 28;

	// circular shifts
	rightCircularShift(left, LCS[15-index]);
	rightCircularShift(right, LCS[15-index]);

	// copying left and right shifted keys to roundKey.
	roundKey = left;
	roundKey <<= 28;
	roundKey += right;
}
// Preemptively shifting all keys using LCS matrix.
void fullShiftLCS(uint64_t& roundKey)
{
	uint32_t left, right;
	uint64_t mask28Bits = 268435455; // covers first 28 bits

	// getting left and right sides
	right = roundKey & mask28Bits;
	mask28Bits <<= 28;
	mask28Bits = roundKey & mask28Bits;
	left = mask28Bits >> 28;

	uint32_t numShifts = 0;
	for (int i = 0; i < 16; i++)
	{
		numShifts += LCS[i];
	}
	// circular shifts
	leftCircularShift(left,numShifts);
	leftCircularShift(right, numShifts);

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
void swapLR(uint64_t& input) // Swap left (32 bit) and right (32 bit) parts of the 64 bit input.
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

// Essential functions
void generateRoundKeys(uint64_t* keys)
{
	uint64_t roundKey;
	uint64_t key;
	generateKey(key);
	for (int i = 0; i < 16; i++)
	{
		generateRoundKey(i, roundKey);
		roundKeyPermutation(roundKey);
		keys[i] = roundKey;
	}
}
void InitKeyDES(uint64_t* keys)
{
	generateRoundKeys(keys);
}

void EncryptDES(const uint64_t& plaintext, const uint64_t* keys, uint64_t& decryption)
{
	uint64_t& result = decryption; // setting alias for decryption

	uint64_t input = plaintext;
	uint64_t roundKey;
	uint64_t permutedRoundKey;
	uint64_t left; // last 32 bits of plaintext/input to algorithm are preserved in this variable 

	initialPermutation(input);

	for (int i = 0; i < 16; i++)
	{
		// Result[63:32] = Input[31:0];
		result = input;
		result <<= 32;
		// preserve left side
		left = input >> 32;

		// round key
		permutedRoundKey = keys[i];

		// Expansion permutation
		expandPermutation(input); // 48 bits

		// XOR with permuted round key
		input ^= permutedRoundKey;

		// Substitution S-boxes
		substitute(input); // 32 bits
		
		// "P-matrix" permutation i.e. mix/shuffle
		mixPermutation(input); 

		result += left^input; // Result[31:0] = L XOR f[31:0];

		input = result;
	}

	swapLR(result);
	reverseInitialPermutation(result);
}
void DecryptDES(const uint64_t& encryption, const uint64_t* keys, uint64_t& decryption)
{
	uint64_t input = encryption;
	uint64_t roundKey;
	uint64_t permutedRoundKey;
	uint64_t& result = decryption;
	uint64_t left;

	// initial operations
	initialPermutation(input);

	for (int i = 0; i < 16; i++)
	{
		// Result[63:32] = Input[31:0];
		result = input;
		result <<= 32;
		// preserve left side
		left = input >> 32;

		// round key
		permutedRoundKey = keys[15 - i];

		// Expansion
		expandPermutation(input); // 48 bits
		// XOR with key
		input ^= permutedRoundKey;

		// Substitution 
		substitute(input); // 32 bits

		// "P matrix" permutation i.e. shuffle/mix permutation
		mixPermutation(input);
		result += left ^ input; // Result[31:0] = L XOR f[31:0];

		input = result;
	}

	swapLR(result);
	reverseInitialPermutation(result);
}


// Testing function
void foo()
{
	const int numTests = 524288; // number of tests to generate 4MB of plaintext.
	uint64_t keys[16];
	uint64_t plaintext; 
	uint64_t encryption, decryption;
	uint64_t* plaintexts = new uint64_t[numTests];
	int bFlag = 0;

	//prep plaintexts
	for (int i = 0; i < numTests; i++)
	{
		plaintexts[i] = ((uint64_t)rand()) << 32 | rand();
	}

	// running tests on Encryption/Decryption validation on random values of plaintext.
	clock_t start = clock();
	for (int i = 0; i < numTests; i++)
	{
		plaintext = plaintexts[i];
		InitKeyDES(&keys[0]);
		EncryptDES(plaintext, &keys[0], encryption);
		DecryptDES(encryption, &keys[0], decryption);

		if (plaintext!=decryption)
		{
			bFlag = 1;
			break;
		}
	}
	clock_t end = clock();
	double timeDiff = (double)(end - start) / (CLOCKS_PER_SEC);

	double sizeBytes = numTests * 8; // 8 bytes of plaintext
	double avgTime = timeDiff / numTests;
	double sizeMegaBytes = sizeBytes / 1048576;
	double speed = sizeMegaBytes / (timeDiff);

	std::cout << "Was encryption/decryption successful? " << (bFlag ? "false" : "true") << "\n";
	std::cout << "Total time to encrypt + decrypt: " << timeDiff << "s\n";
	std::cout << "Total bytes (encrypted+decrypted): " << sizeBytes << " bytes\n";
	std::cout << "Average time to encrypt + decrypt: " << (timeDiff*1000*1000) / numTests << "us\n";
	std::cout << "Average speed to encrypt + decrypt: " << speed << "MBPS (Megabytes Per Second)\n";
	std::cout << "Average speed to encrypt + decrypt: " << speed * 8 << "MbPS (Megabits Per Second)\n";

	// multithread


}



