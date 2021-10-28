// Yuval Bar 2021

#include "search.h"

#include <iostream>

int main()
{
    // n is the number of rows/columns (must be a square grid)
    const int n = 3;

	gridi solvedGrid(n, std::vector<int>(n));

    std::cout << "Solved grid:\n";

	for (auto i = 0; i < n; ++i)
	{
		for (auto j = 0; j < n; ++j)
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

	std::vector<gridi> testGrids = {
		{
			// Depth 0
			{1,2,3},
			{4,5,6},
			{7,8,0}
		},
		{
			// Depth 2
			{1,2,3},
			{4,5,6},
			{0,7,8}
		},
		{
			// Depth 4
			{1,2,3},
			{5,0,6},
			{4,7,8}
		},
		{
			// Depth 8
			{1,3,6},
			{5,0,2},
			{4,7,8}
		},
		{
			// Depth 12
			{1,3,6},
			{5,0,7},
			{4,8,2}
		},
		{
			// Depth 16
			{1,6,7},
			{5,0,3},
			{4,8,2}
		},
		{
			// Depth 20
			{7,1,2},
			{4,8,5},
			{6,3,0}
		},
		{
			// Depth 24
			{0,7,2},
			{4,6,1},
			{3,5,8}
		},
	};

	gridi tempGrid = {
		{1,2,3},
		{4,5,6},
		{7,8,0}
	};

	gridi initialGrid = testGrids[7];

	std::cout << "initial grid:\n";

	for (unsigned int i = 0; i < initialGrid.size(); ++i)
	{
		for (unsigned int j = 0; j < initialGrid[0].size(); ++j)
		{
			std::cout << initialGrid[i][j] << '\t';
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;

	if (1)
	{
		Search search(initialGrid, solvedGrid);
		search.runAstarMisplacedTileSearch();
	}

	int input = 0;
	std::cin >> input;
}