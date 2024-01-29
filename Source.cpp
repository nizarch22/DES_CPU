#include <iostream>
#include "DES.h"

unsigned int P1[] = 
{	
	2,3,4,5,10,
	9,8,7,6,1
};


int main()
{
	uint64_t input = 225;
	printMatrix(input, 2, 5);

	unsigned int P[2][5] = { {2,3,4,5,10},{9,8,7,6,1} };

	permuteMatrix(&input, (unsigned int*)P, sizeof(P1)/sizeof(unsigned int));
	printMatrix(input, 2, 5);
}