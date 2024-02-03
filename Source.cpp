#include <iostream>
#include "DES.h"

unsigned int P0[] = 
{	
	2,3,4,5,10,
	9,8,7,6,1
};

void printMatrixarr(unsigned int* P, int y, int x)
{
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			std::cout << P[i*x+j] << ",";
		}
		std::cout << "\n";
	}
	std::cout << "Matrix array printed.\n";
}
int main()
{
	uint64_t input = 225;

	// equiv matrix test
	//uint64_t input1 = 225;
	//int a = bEqualMatrix(input, input1, 64);
	//std::cout << "Result from a: " << a << "\n";

	// calculate reverseIP:
	//printMatrixarr(IP, 8, 8);
	//unsigned int RIP[64];
	//for (int i = 0; i < 64; i++)
	//{
	//	RIP[IP[i]-1] = i + 1;
	//}
	//printMatrixarr(RIP, 8, 8);
	//printMatrix(input, 2, 5);

	//unsigned int P[2][5] = { {2,3,4,5,10},{9,8,7,6,1} };

	//permuteMatrix(&input, (unsigned int*)P, sizeof(P1)/sizeof(unsigned int));
	//printMatrix(input, 2, 5);
	foo();
}