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

	gridi initialGrid = {
		{0,7,2},
		{4,6,1},
		{3,5,8}
	};

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

	Search search(initialGrid, solvedGrid);
	search.runAstarManhattanDistanceSearch();

	// @TODO: Deallocate all resources
}