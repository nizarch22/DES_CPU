#pragma once

void EncryptDES();
void DecryptDES();

// debug functions
void printMatrix(uint64_t matrix, int y, int x);

//- including will not be necessary after debugging is done.
// matrix helper functions 
void permuteMatrix(uint64_t* input, unsigned int* P, unsigned int size);