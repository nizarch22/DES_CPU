#include <iostream>
#include "DES.h"
#include <chrono>
#include <future>
#include <vector>
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


// multithreading test
int N = 1<<5;
std::vector<int*> a;
std::vector<int*> b;
std::vector<int*> res1;
std::vector<int*> res;

//std::mutex mu;
int matrixMult(int* a, int* b, int* res, int N)
{
	int sum = 0;
	int loc;
	int locA, locB;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			loc = j + i * N;
			sum = 0;

			for (int k = 0; k < N; k++)
			{
				locA = i * N + k;
				locB = k * N + j;
				sum += a[locA] * b[locB];
			}
			res[loc] = sum;
		}
	}
	return 1;
}

static void mult(int i)
{
	matrixMult(a[i], b[i], res[i], N);
}


void randomize(int* a, int N)
{
	for (int i = 0; i < N * N; i++)
	{
		a[i] = rand();
	}
}



int main()
{
	uint64_t input = 225;


	foo();
	return 0;

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
	//foo();


	for (int i = 0; i < 1000; i++)
	{
		int* matA = new int[N*N];
		int* matB = new int[N*N];
		int* matC = new int[N*N];
		int* matC1 = new int[N*N];
		a.push_back(matA);
		b.push_back(matB);
		res.push_back(matC);
		res1.push_back(matC1);

		randomize(a[i], N);
		randomize(b[i], N);
	}


	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 1000; i++)
	{
		matrixMult(a[i], b[i], res1[i], N);
	}
	auto end = std::chrono::high_resolution_clock::now();
	// retrieve timespan
	auto timeDiff = std::chrono::duration_cast<std::chrono::duration<double>> (end - start);
	double timeSpanNormal = timeDiff.count();

	//initialize future vector
	std::vector<std::future<void>> futures;

	// thread function call

	std::vector<std::thread> threads;
	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 1000; i++)
	{
		//futures.push_back(std::async(std::launch::async, mult, i));
		std::thread t(mult, i);
		threads.push_back(std::move(t));
	}

	for (int i = 0; i < 1000; i++)
	{
		//futures[i].wait();
		threads[i].join();
	}
	end = std::chrono::high_resolution_clock::now();

	// retrieve timespan
	timeDiff = std::chrono::duration_cast<std::chrono::duration<double>> (end - start);
	double timeSpan = timeDiff.count();

	// testing results
	int flag = 0;
	for (int i = 0; i < 1000; i++)
	{
		for (int k = 0; k < N * N; k++)
		{
			if (res[i][k]!=res1[i][k])
			{
				int f = (res[i][k]);
				std::cout << "\nERROR!\n";
				flag = 1;
				break;
			}
		}
		if (flag)
			break;
	}

	std::cout << "Normal runtime: " << timeSpanNormal<< "\n";
	std::cout << "Multithread runtime: " << timeSpan << "\n";
	std::cout << "Improvement runtime: " << timeSpanNormal/timeSpan << "\n";

}