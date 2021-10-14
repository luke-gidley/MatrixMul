#include <iostream>
#include <fstream>
#include <vector>
#include <thread>


using namespace std;

void multiply(int** a, int** b, int** result, unsigned int matrixRowLength, unsigned int firstMatrixColLength, unsigned int secondMatrixColLength);

int main() 
{
	unsigned int row1, col1, row2, col2;
	int **a, **b, **result;
	unsigned int i, j;
	unsigned int nThreads;

	fstream first("first.txt", fstream::in);
	fstream second("second.txt", fstream::in);
	fstream resultFile("result.txt", fstream::out);

	vector<thread> threadVector;

	if (!first.is_open() || !second.is_open())
	{
		cout << "unable to open files." << endl;
		exit(1);
	}

	first >> row1 >> col1;
	second >> row2 >> col2;

	if (col1 != row2)
	{
		cout << "cannot multiply matrix with dimensions " << row1 << " X " << col1 << " and " << row2 << " X " << col2 <<endl;
	}

	a = new int* [row1];
	for (i = 0; i < row1; i++)
	{
		a[i] = new int[col1];

		for (j = 0; j < col1; j++)
		{
			if (first.eof())
			{
				cout << "Unable to read enough data from the matrix" << endl;
			}
			first >> a[i][j];
		}
	}

	b = new int* [row2];
	for (i = 0; i < row2; i++)
	{
		b[i] = new int[col2];

		for (j = 0; j < col2; j++)
		{
			if (first.eof())
			{
				cout << "Unable to read enough data from the matrix" << endl;
			}
			second >> b[i][j];
		}
	}


	result = new int* [row1];
	for (int i = 0; i < row1; i++)
	{
		result[i] = new int[col2];
		for (int j = 0; j < col2; j++)
		{
			result[i][j] = 0;
		}
	}


	multiply(a, b, result, row1, col1, col2);

	resultFile << row1 << " " << col2 << endl;
	for (i = 0; i < row1; i++)
	{
		for (j = 0; j < col2; j++)
			resultFile << result[i][j] << " ";

		resultFile << endl;
	}

	first.close();
	second.close();
	resultFile.close();


	return 0;

}


void multiply(int** a, int** b, int** result, unsigned int matrixRowLength, unsigned int firstMatrixColLength, unsigned int secondMatrixColLength)
{
	for (unsigned int rows = 0; rows < matrixRowLength; rows++)
	{
		for (unsigned int cols = 0; cols < secondMatrixColLength; cols++)
		{
			for (int i = 0; i < firstMatrixColLength; i++)
			{
				result[rows][cols] += a[rows][i] * b[i][cols];
			}
		}
	}
}


