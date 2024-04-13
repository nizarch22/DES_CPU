#include <iostream>
#include <chrono>
#include <random>
#include <time.h>
#include "DES.h"
#include "DES_Matrices_NIST.h"
/////////////////////////////////////////////////////////////////////////////////////
// permutation - substitution functions
/////////////////////////////////////////////////////////////////////////////////////
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
void initialPermutation(uint64_t& input)
{
	permuteMatrix(input, IP, 64);
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

/////////////////////////////////////////////////////////////////////////////////////
// key generation functions
/////////////////////////////////////////////////////////////////////////////////////
void generateKey(uint64_t& key)
{
	// 64 bits
	key = ((uint64_t)rand()) << 32 | rand();
}
void leftCircularShift(uint32_t& input, uint8_t times)
{
	uint32_t mask28thBit = 1 << 27; // 28th bit
	uint32_t mask28Bits = 268435455; // covers first 28 bits

	uint8_t bit;
	for (int i = 0; i < times; i++)
	{
		bit = (input & mask28thBit) >> 27;
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
void generateShiftedKey(const int& index, uint64_t& roundKey)
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
	rightCircularShift(left, LCS[15 - index]);
	rightCircularShift(right, LCS[15 - index]);

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
	leftCircularShift(left, numShifts);
	leftCircularShift(right, numShifts);

	// copying left and right shifted keys to roundKey.
	roundKey = left;
	roundKey <<= 28;
	roundKey += right;
}
void generateRoundKeys(uint64_t* roundKeys, const uint64_t& key)
{
	// Calculate all round keys
	uint64_t roundKey;
	uint64_t shiftedKey = key; // shifted keys are reused for the next rounds
	for (int i = 0; i < 16; i++)
	{
		// 28 bit left shifting
		generateShiftedKey(i, shiftedKey);

		// permutation of round key
		roundKey = shiftedKey;
		roundKeyPermutation(roundKey);
		roundKeys[i] = roundKey;
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// Debug functions
/////////////////////////////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////////////////////////////
// Essential functions
/////////////////////////////////////////////////////////////////////////////////////
void InitKeysDES(uint64_t* roundKeys)
{
	// generate a random key and perform PC1 permutation
	uint64_t key;
	generateKey(key);
	permuteMatrix(key, PC1, 56);

	generateRoundKeys(roundKeys, key);
}
void EncryptDES(const uint64_t& plaintext, const uint64_t* roundKeys, uint64_t& encryption)
{
	uint64_t& result = encryption; // setting alias for decryption

	uint64_t input = plaintext;
	uint64_t roundKey;
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
		roundKey = roundKeys[i];

		// Expansion permutation
		expandPermutation(input); // 48 bits

		// XOR with permuted round key
		input ^= roundKey;

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
void DecryptDES(const uint64_t& encryption, const uint64_t* roundKeys, uint64_t& decryption)
{
	uint64_t input = encryption;
	uint64_t roundKey;
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
		roundKey = roundKeys[15 - i];

		// Expansion
		expandPermutation(input); // 48 bits
		// XOR with key
		input ^= roundKey;

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

/////////////////////////////////////////////////////////////////////////////////////
// Testing functions
/////////////////////////////////////////////////////////////////////////////////////
void foo()
{
}

void foo2()
{
	//uint64_t keys[16];
	//uint64_t msg = 4702111234474983745;
	//uint64_t key = 4774451407313060418;
	//uint64_t encrypt;

	//permuteMatrix(key, PC1, 56);
	//uint64_t roundKey = key;
	//uint64_t roundKeyPerm;
	//for (int i = 0; i < 16; i++)
	//{
	//	generateRoundKey(i, roundKey);
	//	roundKeyPerm = roundKey;
	//	roundKeyPermutation(roundKeyPerm);
	//	keys[i] = roundKeyPerm;
	//	//printMatrix(roundKey, 1, 56);
	//}
	//EncryptDES(msg, keys, encrypt);
	//char buff[17]; buff[16] = 0;
	////std::cout << msg;
	//uint64_t mask = 17293822569102704640;
	//uint64_t res;
	//uint64_t encryption = encrypt;
	//for (int i = 0; i < 16; i++)
	//{
	//	res = encrypt & mask;
	//	res >>= 60;
	//	if (res >= 10)
	//	{
	//		res += 55;
	//	}
	//	if (res < 10)
	//	{
	//		res += 48;
	//	}
	//	buff[i] = (char)res;
	//	if (i % 2 == 0 && i > 0)
	//		printf(" ");
	//	printf("%c", buff[i]);
	//	encrypt <<= 4;
	//}
	//uint64_t decrypt;
	//DecryptDES(encryption, keys, decrypt);
	//int bSuccess = (msg == decrypt);
	//std::cout << "\nSuccess? " << (bSuccess ? "Yes":"No") << "\n" << decrypt << "\n";
	//printMatrix(encryption, 8, 8);
	//std::cout << encryption;

	//uint64_t newMsg = 6339430835082086936;
	//DecryptDES(newMsg, keys, decrypt);
	//std::cout << "\n" << decrypt << "\n";

	//printf("\n%s\n", buff);
}

//unsigned int P0[] =
//{
//	2,3,4,5,10,
//	9,8,7,6,1
//};
//
//void printMatrixarr(unsigned int* P, int y, int x)
//{
//	for (int i = 0; i < y; i++)
//	{
//		for (int j = 0; j < x; j++)
//		{
//			std::cout << P[i * x + j] << ",";
//		}
//		std::cout << "\n";
//	}
//	std::cout << "Matrix array printed.\n";
//}
void foo3()
{
	//uint64_t input = 225;

	// equiv matrix test
	//uint64_t input1 = 225;
	//int a = bEqualMatrix(input, input1, 64);
	//std::cout << "Result from a: " << a << "\n";

	// calculate reverseIP:
	//printMatrixarr(IP, 8, 8);
	//unsigned int RIP[64];
	//for (int i = 0; i < 64; i++)
	//{
	//	RIP[IP[i]-1] = i + 1;
	//}
	//printMatrixarr(RIP, 8, 8);
	//printMatrix(input, 2, 5);

	//unsigned int P[2][5] = { {2,3,4,5,10},{9,8,7,6,1} };

	//permuteMatrix(&input, (unsigned int*)P, sizeof(P1)/sizeof(unsigned int));
	//printMatrix(input, 2, 5);
}


