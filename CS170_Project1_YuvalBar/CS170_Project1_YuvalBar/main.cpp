// Yuval Bar 2021

#include "search.h"

#include <iostream>

int main()
{
    // n is the number of rows/columns (must be a square grid)
    const int n = 3;

	int** solvedGrid = new int* [n];
	for (auto i = 0; i < n; ++i)
	{
		solvedGrid[i] = new int[n];
	}

    std::cout << "Solved grid:\n";

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
            if (i == n - 1 && j == n - 1)
            {
                solvedGrid[i][j] = 0;
            }
            else
            {
                solvedGrid[i][j] = (i * n) + j + 1;
            }
			
            std::cout << solvedGrid[i][j] << '\t';
		}
        std::cout << std::endl;
	}

    std::cout << std::endl;

    int initialGrid[][3] = {
        {1,3,6},
        {5,0,7},
        {4,8,2}
    };

	int** initialGridPtr = new int* [n];
	for (auto i = 0; i < n; ++i)
	{
        initialGridPtr[i] = new int[n];
	}

	std::cout << "initial grid ptr:\n";

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
            initialGridPtr[i][j] = initialGrid[i][j];
			std::cout << initialGridPtr[i][j] << '\t';
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;

	Search search(initialGridPtr, solvedGrid);
	search.runUniformCostSearch();

	// @TODO: Deallocate all resources
}