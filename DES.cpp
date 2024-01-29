#include <iostream>


uint64_t key;
uint64_t plaintext;

void encryptDES()
{
	generateKey();
	initialPermutation();
	// result = input >> 32; // i.e. Result[31:0] = Input[63:32]
	for (int i = 0; i < 16; i++)
	{
		generateRoundKey();
		expandPermutation(); // result1
		// result2 = result1^roundKey;
		Substitute(); // result3
		Transpose(); // result 4
		// xxx
	}
	// input <<=32; input>>=32; // i.e. making input[63:32] = 0
	//result += input // i.e. Result[63:32] = Input[31:0] ^ Result4[31:0]
}

void decryptDES()
{

}


void initialPermutation()
{

}



void generateKey()
{
	// 64 bits
}


void generateRoundKey()
{
}

void expandPermutation()
{

}

void Substitute()
{

}


void Transpose()
{

}












