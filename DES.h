#pragma once

void EncryptDES(const uint64_t& plaintext, const uint64_t* roundKeys, uint64_t& encryption);
void DecryptDES(const uint64_t& encryption, const uint64_t* roundKeys, uint64_t& decryption);
void InitKeysDES(uint64_t* roundKeys, uint64_t key);

// debug functions
void printMatrix(uint64_t matrix, int y, int x);
void foo();

//- including will not be necessary after debugging is done.
// matrix helper functions 
void permuteMatrix(uint64_t& input, const unsigned char* P, const unsigned int size);
int bEqualMatrix(const uint64_t& m1, const uint64_t& m2, const int size);
