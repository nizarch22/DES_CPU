#pragma once

void encryptDES();
void decryptDES();
void initialPermutation();
void generateKey();
void generateRoundKey();
void expandPermutation();
void Substitute();
void Transpose();

// matrix helper functions
void permuteMatrix(uint64_t* input, unsigned int* P, unsigned int size);


// debug functions
void printMatrix(uint64_t matrix, int y, int x);