#pragma once

void encryptDES();
void decryptDES();
void initialPermutation();
void generateKey();
void generateRoundKey();
void expandPermutation();
void Substitute();
void Transpose();



// debug functions
void printMatrix(int y, int x, uint64_t matrix);