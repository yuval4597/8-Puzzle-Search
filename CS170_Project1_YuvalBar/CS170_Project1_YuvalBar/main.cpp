// Yuval Bar 2021

#include "search.h"

#include <iostream>
#include <chrono>
#include <fstream>

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

int main()
{
	bool shouldRunPerformanceTests = false;

	char c;
	std::cout << "Would you like to enter an initial state yourself? (y/n) ";	// Secret option 'p' for running performance tests
	std::cin >> c;

	while (c != 'y' && c != 'n' && c != 'p')
	{
		std::cout << "Invalid input. Would you like to enter an initial state yourself? (y/n) ";
		std::cin >> c;
	}

	// n is the number of rows/columns (must be a square grid)
	int n = 3;

	gridi tempGrid = {
		{3,1,8},
		{5,6,4},
		{2,7,0}
	};

	gridi initialGrid;

	if (c == 'n')
	{
		std::cout << "Running with default initial state...\n\n";
		initialGrid = tempGrid;
	}
	else if (c == 'y')
	{
		std::cout << "Enter n (where grid is nxn): ";
		std::cin >> n;
		while (n <= 0)
		{
			std::cout << "Invalid input. Enter n (where grid is nxn): ";
			std::cin >> n;
		}

		std::cout << "Enter grid:\n";
		for (auto i = 0; i < n; ++i)
		{
			std::vector<int> tempRow;
			for (auto j = 0; j < n; ++j)
			{
				int input;
				std::cin >> input;
				tempRow.push_back(input);
			}
			initialGrid.push_back(tempRow);
		}
	}
	else // c == 'p'
	{
		shouldRunPerformanceTests = true;
		n = 3;
	}

	if (shouldRunPerformanceTests)
	{
		int queuingFnType = 0;
		std::cout << "Run performance test using\n1. Uniform Cost Search\n2. A* Misplaced Tile\n3. A* Manhattan Distance\nYour choice: ";
		std::cin >> queuingFnType;

		while (queuingFnType < 1 || queuingFnType > 3)
		{
			std::cout << "Invalid input. Run performance test using\n1. Uniform Cost Search\n2. A* Misplaced Tile\n3. A* Manhattan Distance\nYour choice: ";
			std::cin >> queuingFnType;
		}

		// Output files
		std::ofstream timefile("TimeData.txt");
		std::ofstream spacefile("SpaceData.txt");

		// Go through all test grids
		for (auto i = 0; i < testGrids.size(); ++i)
		{
			initialGrid = testGrids[i];
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
			
			auto startTime = std::chrono::system_clock::now();
			switch (queuingFnType)
			{
			case 1:
				search.runUniformCostSearch();
				break;
			case 2:
				search.runAstarMisplacedTileSearch();
				break;
			case 3:
				search.runAstarManhattanDistanceSearch();
				break;
			default:
				std::cout << "Something went wrong :(\n";
			}

			std::cout << "\nPerformance:\n";

			// Time
			auto endTime = std::chrono::system_clock::now();
			auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
			std::cout << "Took " << elapsedTime.count() << " milliseconds to find a solution.\n";

			// Space (num of nodes)
			std::cout << "Encountered " << search.getNumEncounteredNodes() << " nodes to reach this solution.\n\n";

			if (timefile.is_open())
			{
				timefile << elapsedTime.count() << ", ";
			}

			if (spacefile.is_open())
			{
				spacefile << search.getNumEncounteredNodes() << ", ";
			}
		}

		if (timefile.is_open())
		{
			timefile.close();
		}

		if (spacefile.is_open())
		{
			spacefile.close();
		}
	}
	else
	{
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

		int queuingFnType = 0;
		std::cout << "Solve using\n1. Uniform Cost Search\n2. A* Misplaced Tile\n3. A* Manhattan Distance\nYour choice: ";
		std::cin >> queuingFnType;

		while (queuingFnType < 1 || queuingFnType > 3)
		{
			std::cout << "Invalid input. Solve using\n1. Uniform Cost Search\n2. A* Misplaced Tile\n3. A* Manhattan Distance\nYour choice: ";
			std::cin >> queuingFnType;
		}

		auto startTime = std::chrono::system_clock::now();
		switch (queuingFnType)
		{
		case 1:
			search.runUniformCostSearch();
			break;
		case 2:
			search.runAstarMisplacedTileSearch();
			break;
		case 3:
			search.runAstarManhattanDistanceSearch();
			break;
		default:
			std::cout << "Something went wrong :(\n";
		}

		std::cout << "\nPerformance:\n";

		// Time
		auto endTime = std::chrono::system_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
		std::cout << "Took " << elapsedTime.count() << " milliseconds to find a solution.\n";

		// Space (num of nodes)
		std::cout << "Encountered " << search.getNumEncounteredNodes() << " nodes to reach this solution.\n\n";
	}
}