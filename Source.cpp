#include <stdio.h>
#include <iostream>

#include "DES.h"

int main()
{
	// Testing the speed and time of the DES algorithm.
	const int numTests = 524288; // number of tests to get 4MB plaintext
	const int bytesMessages = sizeof(uint64_t) * numTests;
	uint64_t* plaintexts = (uint64_t*)malloc(bytesMessages);
	uint64_t* keys = (uint64_t*)malloc(bytesMessages);
	uint64_t* encryptions = (uint64_t*)malloc(bytesMessages);
	uint64_t* decryptions = (uint64_t*)malloc(bytesMessages);
	for (int i = 0; i < numTests; i++)
	{
		plaintexts[i] = ((uint64_t)rand()) << 32 | rand();
		keys[i] = ((uint64_t)rand()) << 32 | rand();
		encryptions[i] = 0;
		decryptions[i] = 0;
	}

	// running tests on Encryption/Decryption validation on randomly generated plaintexts.
	clock_t start = clock();
	for (int i = 0; i < numTests; i++)
	{
		EncryptDES(plaintexts[i], keys[i], encryptions[i]);
		//DecryptDES(encryptions[i], keys[i], decryptions[i]);
	}
	clock_t end = clock();

	// Check if decryptions match messages
	bool bValid = 1;
	for (int i = 0; i < numTests; i++)
	{
		DecryptDES(encryptions[i], keys[i], decryptions[i]);
		bValid &= (decryptions[i] == plaintexts[i]);
	}
	if (!bValid)
	{
		std::cout << "Decryption-message Mismatch!\n";
		return -1;
	}
	double timeDiff = (double)(end - start) / CLOCKS_PER_SEC;

	double sizeBytes = numTests * 8; // 8 bytes of plaintext
	//double avgTime = timeDiff.count() / numTests;
	double sizeMegaBytes = sizeBytes / 1048576;
	//double speed = sizeMegaBytes / (timeDiff.count());
	double speed = sizeMegaBytes / (timeDiff);

	std::cout << "Total encrypted size: " << sizeMegaBytes << "Megabytes\n";
	std::cout << "Average speed to encrypt: " << speed * 8 << "MBPS (Megabits Per Second)\n";
	
	return 0;
}