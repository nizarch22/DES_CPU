#pragma once

void EncryptDES(const uint64_t& plaintext, const uint64_t& key, uint64_t& result, uint64_t keys[16]);
void DecryptDES(uint64_t& encryption, uint64_t keys[16], uint64_t& decryption);

// debug functions
void printMatrix(uint64_t matrix, int y, int x);
void foo();

//- including will not be necessary after debugging is done.
// matrix helper functions 
void permuteMatrix(uint64_t& input, const unsigned char* P, const unsigned int size);
int bEqualMatrix(const uint64_t& m1, const uint64_t& m2, const int size);
