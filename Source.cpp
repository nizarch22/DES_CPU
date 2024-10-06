#include <iostream>
#include "DES.h"

int main()
{
	// Testing the speed and timing of the DES algorithm with key extension (pre feistel loop calculated round keys).
	// The calculation time of round keys is still included!

	clock_t beginning = clock();
	const int numTests = 524288; // number of tests to generate 4MB of plaintext.
	const int numRoundKeys = 8388608; // 16*numTests

	uint64_t encryption, decryption;
	uint64_t* roundKeys = new uint64_t[numRoundKeys];
	uint64_t* plaintexts = new uint64_t[numTests];
	uint64_t* encryptions = new uint64_t[numTests];
	uint64_t* keys = new uint64_t[numTests];
	int bValid = 1;

	// looad plaintexts and keys
	for (int i = 0; i < numTests; i++)
	{
		plaintexts[i] = ((uint64_t)rand()) << 32 | rand();
		keys[i] = ((uint64_t)rand()) << 32 | rand();
	}

	// running tests on Encryption/Decryption validation on random values of plaintext.
	clock_t start = clock();
	
	for (int i = 0; i < numTests; i++)
	{
		InitKeysDES(&roundKeys[i], keys[i]);
	}
	for (int i = 0; i < numTests; i++)
	{
		EncryptDES(plaintexts[i], &roundKeys[i], encryptions[i]);
	}
	clock_t end = clock();

	for (int i = 0; i < numTests; i++)
	{
		DecryptDES(encryptions[i], &roundKeys[i], decryption);
		bValid &= (plaintexts[i] == decryption);
	}
	if (!bValid)
	{
		std::cout << "Invalid encryption-decryption!\n";
		return -1;
	}
	double timeDiff = (double)(end - start) / (CLOCKS_PER_SEC);

	double sizeBytes = numTests * 8; // 8 bytes of plaintext
	double avgTime = timeDiff / numTests;
	double sizeMegaBytes = sizeBytes / 1048576;
	double speed = sizeMegaBytes / timeDiff;

	std::cout << "Total encrypted size: " << sizeMegaBytes << "Megabytes\n";
	std::cout << "Average speed to encrypt: " << speed * 8 << "MBPS (Megabits Per Second)\n";

}