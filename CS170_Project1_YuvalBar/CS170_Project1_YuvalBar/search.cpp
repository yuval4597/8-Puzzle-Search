#include "search.h"

#include <iostream>

Search::Search(int** initialState, int** goalState)
{
	const int numOfOperators = 4;	// Up, down, left, right
	std::function<bool(Search&, int**, const int&)>* operatorList = new std::function<bool(Search&, int**, const int&)>[numOfOperators];
	operatorList[0] = &Search::movePieceDown;
	operatorList[1] = &Search::movePieceUp;
	operatorList[2] = &Search::movePieceLeft;
	operatorList[3] = &Search::movePieceRight;

	problem.initialState = initialState;
	problem.goalState = goalState;
	problem.operators = operatorList;
}

void Search::uniformCostSearch(std::priority_queue < Node*, std::vector<Node*>, decltype(costComparisonLambda) >& outNodes, std::function<bool(Search&, int**, const int&)>* inOperators)
{
	if (outNodes.empty())
	{
		return;
	}

	Node* currentNode = outNodes.top();
	exploredGrids.push_back(copyGrid(currentNode->state, 3));	// @TODO: Hardcoded value of n

	for (auto i = 0; i < 4; ++i)	// @TODO: Hardcoded value of numOfOperators
	{
		int** copiedState = copyGrid(currentNode->state, 3);	// @TODO: Hardcoded value of n
		Node* newNode = new Node(currentNode->cost + 1, copiedState);
		newNode->parent = currentNode;
		// 		std::cout << "New node before inOperator" << i << std::endl;
		// 		printGrid(newNode->state, 3);

		inOperators[i](*this, newNode->state, 3);	// @TODO: Hardcoded value of n

// 		std::cout << "\nNew node after inOperator" << i << std::endl;
// 		printGrid(newNode->state, 3);
// 		std::cout << std::endl;

		if (!wasGridExplored(copiedState, 3))	// @TODO: Hardcoded value of n
		{
			outNodes.push(newNode);
		}
	}

	std::priority_queue < Node*, std::vector<Node*>, decltype(costComparisonLambda) > pq(outNodes);

	/*std::cout << "Current nodes in queue:\n";*/
	while (!pq.empty())
	{
		// 		for (int i = 0; i < 3; ++i)		// @TODO: Hardcoded n value, should be a print function anyway...
		// 		{
		// 			for (int j = 0; j < 3; ++j)	// @TODO: Hardcoded n value, should be a print function anyway...
		// 			{
		// 				std::cout << pq.top()->state[i][j] << '\t';
		// 			}
		// 			std::cout << std::endl;
		// 		}
		// 
		// 		std::cout << std::endl;

		pq.pop();
	}
}

int Search::calculateNumMisplacedTiles(int** grid, const int& n)
{
	// @TODO: This should simply compare to the goal grid
	int numMisplaced = 0;

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			if (grid[i][j] == 0)
			{
				// Not counting blank piece as misplaced tile
				continue;
			}

			// Check explicitly if the last piece is not the 0 (blank) piece, others are straightforward
			if ((i == n - 1 && j == n - 1) ||
				grid[i][j] != (i * n) + j + 1)
			{
				++numMisplaced;
			}
		}
	}

	return numMisplaced;
}

void Search::aStarMisplacedTile(std::priority_queue < Node*, std::vector<Node*>, decltype(costComparisonLambda) >& outNodes, std::function<bool(Search&, int**, const int&)>* inOperators)
{
	if (outNodes.empty())
	{
		return;
	}

	Node* currentNode = outNodes.top();
	exploredGrids.push_back(copyGrid(currentNode->state, 3));	// @TODO: Hardcoded value of n

	for (auto i = 0; i < 4; ++i)	// @TODO: Hardcoded value of numOfOperators
	{
		int** copiedState = copyGrid(currentNode->state, 3);	// @TODO: Hardcoded value of n

		// Costs
		int g = currentNode->cost + 1;
		int h = calculateNumMisplacedTiles(copiedState, 3);	// @TODO: Hardcoded value of n
		int finalCost = h + g;

		Node* newNode = new Node(finalCost, copiedState);
		newNode->parent = currentNode;
		// 		std::cout << "New node before inOperator" << i << std::endl;
		// 		printGrid(newNode->state, 3);

		inOperators[i](*this, newNode->state, 3);	// @TODO: Hardcoded value of n

// 		std::cout << "\nNew node after inOperator" << i << std::endl;
// 		printGrid(newNode->state, 3);
// 		std::cout << std::endl;

		if (!wasGridExplored(copiedState, 3))	// @TODO: Hardcoded value of n
		{
			outNodes.push(newNode);
		}
	}

	std::priority_queue < Node*, std::vector<Node*>, decltype(costComparisonLambda) > pq(outNodes);

	/*std::cout << "Current nodes in queue:\n";*/
	while (!pq.empty())
	{
		// 		for (int i = 0; i < 3; ++i)		// @TODO: Hardcoded n value, should be a print function anyway...
		// 		{
		// 			for (int j = 0; j < 3; ++j)	// @TODO: Hardcoded n value, should be a print function anyway...
		// 			{
		// 				std::cout << pq.top()->state[i][j] << '\t';
		// 			}
		// 			std::cout << std::endl;
		// 		}
		// 
		// 		std::cout << std::endl;

		pq.pop();
	}
}

// General search algorithm suggested by Dr. Keogh
void Search::generalSearch(Problem problem, const std::function<void(Search&, std::priority_queue < Node*, std::vector<Node*>, decltype(costComparisonLambda) >&, std::function<bool(Search&, int**, const int&)>*)>& queueingFunction)
{
	std::priority_queue<Node*, std::vector<Node*>, decltype(costComparisonLambda)> nodes(costComparisonLambda);

	// Initialize priority queue with initial state
	Node* initialNode = new Node(0, problem.initialState);
	nodes.push(initialNode);

	// Keep looping until hit one of the return statements (or run out of memory :P)
	while (1)
	{
		if (nodes.empty())
		{
			// @TODO: FAILURE
			std::cout << "temp message: failure\n";
			return;
		}

		Node* currentNode = nodes.top();

		// 		std::cout << "Current node:\n";
		// 		for (int i = 0; i < 3; ++i)		// @TODO: Hardcoded n value, should be a print function anyway...
		// 		{
		// 			for (int j = 0; j < 3; ++j)	// @TODO: Hardcoded n value, should be a print function anyway...
		// 			{
		// 				std::cout << currentNode->state[i][j] << '\t';
		// 			}
		// 			std::cout << std::endl;
		// 		}
		// 
		// 		std::cout << std::endl;

		if (isGoalState(currentNode->state, problem.goalState, 3))	// @TODO: Hardcoded n value
		{
			// @TODO: SUCCESS
			std::cout << "temp message: success\n";

			do
			{
				printGrid(currentNode->state, 3);	// @ TODO: Hardcoded n value
				currentNode = currentNode->parent;
				std::cout << std::endl;
			} while (currentNode->parent);

			return;
		}

		queueingFunction(*this, nodes, problem.operators);
		nodes.pop();
	}
}

bool Search::isGoalState(int** grid, int** goal, const int& n)
{
	for (auto i = 0; i < n; ++i)
	{
		for (auto j = 0; j < n; ++j)
		{
			if (grid[i][j] != goal[i][j])
			{
				return false;
			}
		}
	}

	return true;
}

bool Search::wasGridExplored(int** grid, int n)
{
	if (exploredGrids.empty())
	{
		return false;
	}

	for (auto exploredGrid : exploredGrids)
	{
		bool isMatch = true;
		for (auto i = 0; i < n; ++i)
		{
			for (auto j = 0; j < n; ++j)
			{
				if (grid[i][j] != exploredGrid[i][j])
				{
					isMatch = false;
					break;
				}
			}

			if (!isMatch)
			{
				break;
			}
		}

		if (isMatch)
		{
			return true;
		}
	}

	return false;
}

void Search::printGrid(int** grid, const int& n)
{
	for (auto i = 0; i < n; ++i)
	{
		for (auto j = 0; j < n; ++j)
		{
			std::cout << grid[i][j] << '\t';
		}
		std::cout << std::endl;
	}
}

int** Search::copyGrid(int** state, const int& n)
{
	int** res = new int* [n];
	for (auto i = 0; i < n; ++i)
	{
		res[i] = new int[n];
		for (auto j = 0; j < n; ++j)
		{
			res[i][j] = state[i][j];
		}
	}

	return res;
}

void Search::findZeroPiece(int& outRow, int& outCol, int** grid, const int& n)
{
	outRow = 0;
	outCol = 0;

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			if (grid[i][j] == 0)
			{
				outRow = i;
				outCol = j;
				return;
			}
		}
	}
}

bool Search::movePieceDown(int** grid, const int& n)
{
	// Find 0 "piece" (technically blank spot)
	int row = 0;
	int col = 0;

	findZeroPiece(row, col, grid, n);

	if (row == 0)
	{
		// Cannot move piece down into 0 spot if it is already at the topmost row
		return false;
	}

	std::swap(grid[row][col], grid[row - 1][col]);
	return true;
}

bool Search::movePieceUp(int** grid, const int& n)
{
	// Find 0 "piece" (technically blank spot)
	int row = 0;
	int col = 0;

	findZeroPiece(row, col, grid, n);

	if (row == n - 1)
	{
		// Cannot move piece up into 0 spot if it is already at the bottommost row
		return false;
	}

	std::swap(grid[row][col], grid[row + 1][col]);
	return true;
}

bool Search::movePieceLeft(int** grid, const int& n)
{
	// Find 0 "piece" (technically blank spot)
	int row = 0;
	int col = 0;

	findZeroPiece(row, col, grid, n);

	if (col == n - 1)
	{
		// Cannot move piece left into 0 spot if it is already at the rightmost column
		return false;
	}

	std::swap(grid[row][col], grid[row][col + 1]);
	return true;
}

bool Search::movePieceRight(int** grid, const int& n)
{
	// Find 0 "piece" (technically blank spot)
	int row = 0;
	int col = 0;

	findZeroPiece(row, col, grid, n);

	if (col == 0)
	{
		// Cannot move piece right into 0 spot if it is already at the leftmost column
		return false;
	}

	std::swap(grid[row][col], grid[row][col - 1]);
	return true;
}

void Search::runUniformCostSearch()
{
	generalSearch(problem, &Search::uniformCostSearch);
}

void Search::runAstarMisplacedTileSearch()
{
	generalSearch(problem, &Search::aStarMisplacedTile);
}
