#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <string>

using namespace std;

void multiply(unsigned int, unsigned int,
    int**, int**, int**,
    unsigned int, unsigned int, unsigned int);

int main()
{
    fstream first("first.txt", fstream::in);
    fstream second("second.txt", fstream::in);
    fstream resultFile("result.txt", fstream::out);

    vector<thread> threadVector;

    unsigned int row1, col1, row2, col2;
    int** a, ** b, ** result;
    unsigned int i, j;
    unsigned int nThreads;

    if (!first.is_open() || !second.is_open())
    {
        cout << "Unable to open input file(s)" << endl;
        exit(1);
    }

    first >> row1 >> col1;
    second >> row2 >> col2;

    if (col1 != row2)
    {
        cout << "Unable to perform multiplication between a matrix " << row1 << " X " << col1 << " and a matrix " << row2 << " X " << col2 << endl;
        exit(1);
    }

    a = new int* [row1];
    for (i = 0; i < row1; i++)
    {
        a[i] = new int[col1];

        for (j = 0; j < col1; j++)
        {
            if (first.eof())
            {
                cout << "Unable to read enough data for the first matrix." << endl;
                exit(1);
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
            if (second.eof())
            {
                cout << "Unable to read enough data for the second matrix." << endl;
                exit(1);
            }
            second >> b[i][j];
        }
    }

    result = new int* [row1];
    for (i = 0; i < row1; i++)
    {
        result[i] = new int[col2];

        for (j = 0; j < col2; j++)
            result[i][j] = 0;
    }

    cout << "Enter number of threads: ";
    cin >> nThreads;

    for (i = 0; i < nThreads; i++)
    {
        threadVector.emplace_back(multiply, i, nThreads, a, b, result, row1, col1, col2);
    }

    for (i = 0; i < nThreads; i++)
    {
        threadVector[i].join();
    }

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

void multiply(unsigned int threadIndex, unsigned int nThreads,
    int** a, int** b, int** result, unsigned int row1,
    unsigned int col1, unsigned int col2)
{
    unsigned int startRow, rowCount, endRow;
    unsigned int residualRows;
    unsigned int r, c, in;
    unsigned int i;
    string str;

    rowCount = row1 / nThreads;
    residualRows = row1 % nThreads;

    startRow = 0;
    for (i = 0; i < threadIndex; i++)
    {
        startRow += rowCount;
        if (i < residualRows)
            startRow += 1;
    }

    if (threadIndex < residualRows)
        rowCount++;

    endRow = startRow + rowCount;

    str = "Thread id " + to_string(threadIndex) + ": start row: " + to_string(startRow) + "\n";
    str += "Thread id " + to_string(threadIndex) + ": end row: " + to_string(endRow) + "\n";
    str += "Thread id " + to_string(threadIndex) + ": row count: " + to_string(rowCount) + "\n";

    cout << str;

    for (r = startRow; r < endRow; r++)
        for (c = 0; c < col2; c++)
            for (in = 0; in < col1; in++)
                result[r][c] += a[r][in] * b[in][c];
}

