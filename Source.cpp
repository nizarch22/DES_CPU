#include <iostream>
#include "DES.h"

int main()
{
	// Testing the speed and time of the DES algorithm.

	const int numTests = 1 << 19; // number of tests to get 4MB plaintext
	uint64_t key;
	uint64_t plaintext;
	uint64_t encryption, decryption;
	uint64_t* plaintexts = new uint64_t[numTests];
	int bFlag = 0;

	for (int i = 0; i < numTests; i++)
	{
		plaintexts[i] = ((uint64_t)rand()) << 32 | rand();
	}

	// running a 100 tests on Encryption/Decryption validation on random values of plaintext.
	clock_t start = clock();
	InitKeyDES(key);
	for (int i = 0; i < numTests; i++)
	{
		plaintext = plaintexts[i];
		EncryptDES(plaintext, key, encryption);
		DecryptDES(encryption, key, decryption);

		if (plaintext != decryption)
		{
			bFlag = 1;
			break;
		}
	}
	clock_t end = clock();
	double timeDiff = (double)(end - start) / CLOCKS_PER_SEC;

	double sizeBytes = numTests * 8; // 8 bytes of plaintext
	//double avgTime = timeDiff.count() / numTests;
	double sizeMegaBytes = sizeBytes / 1048576;
	//double speed = sizeMegaBytes / (timeDiff.count());
	double speed = sizeMegaBytes / (timeDiff);

	std::cout << "Was encryption/decryption successful? " << (bFlag ? "false" : "true") << "\n";
	std::cout << "Total time to encrypt + decrypt: " << timeDiff << "s\n";
	std::cout << "Total bytes (encrypted+decrypted): " << sizeBytes << " bytes\n";
	std::cout << "Average time to encrypt + decrypt: " << (timeDiff * 1000 * 1000) / numTests << "us\n";
	std::cout << "Average speed to encrypt + decrypt: " << speed << "MBPS (Megabytes Per Second)\n";
	std::cout << "Average speed to encrypt + decrypt: " << speed * 8 << "MBPS (Megabits Per Second)\n";
}