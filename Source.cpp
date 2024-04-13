#include <iostream>
#include "DES.h"


int main()
{
	// Testing the speed and timing of the DES algorithm with key extension (pre feistel loop calculated round keys).
	// The calculation time of round keys is still included!

	clock_t beginning = clock();
	const int numTests = 524288; // number of tests to generate 4MB of plaintext.
	uint64_t roundKeys[16];
	uint64_t plaintext;
	uint64_t encryption, decryption;
	uint64_t* plaintexts = new uint64_t[numTests];
	int bFlag = 0;

	// looad plaintexts
	for (int i = 0; i < numTests; i++)
	{
		plaintexts[i] = ((uint64_t)rand()) << 32 | rand();
	}

	// running tests on Encryption/Decryption validation on random values of plaintext.
	clock_t start = clock();
	for (int i = 0; i < numTests; i++)
	{
		plaintext = plaintexts[i];
		InitKeysDES(&roundKeys[0]);
		EncryptDES(plaintext, &roundKeys[0], encryption);
		DecryptDES(encryption, &roundKeys[0], decryption);

		if (plaintext != decryption)
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
	std::cout << "Average time to encrypt + decrypt: " << (timeDiff * 1000 * 1000) / numTests << "us\n";
	std::cout << "Average speed to encrypt + decrypt: " << speed << "MBPS (Megabytes Per Second)\n";
	std::cout << "Average speed to encrypt + decrypt: " << speed * 8 << "MbPS (Megabits Per Second)\n";

	double totalTime = (double)(end - beginning) / (CLOCKS_PER_SEC);
	std::cout << "Total time from start to finish:" << totalTime << "s\n";
}