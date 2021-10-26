#include "search.h"

#include <iostream>

Search::Search(gridi initialState, gridi goalState)
{
	const int numOfOperators = 4;	// Up, down, left, right
	std::function<bool(Search&, gridi&)>* operatorList = new std::function<bool(Search&, gridi&)>[numOfOperators];
	operatorList[0] = &Search::movePieceDown;
	operatorList[1] = &Search::movePieceUp;
	operatorList[2] = &Search::movePieceLeft;
	operatorList[3] = &Search::movePieceRight;

	problem.initialState = initialState;
	problem.goalState = goalState;
	problem.operators = operatorList;
}

void Search::uniformCostSearch(std::priority_queue < Node*, std::vector<Node*>, decltype(costComparisonLambda) >& outNodes, std::function<bool(Search&, gridi&)>* inOperators)
{
	if (outNodes.empty())
	{
		return;
	}

	Node* currentNode = outNodes.top();
	exploredGrids.push_back(currentNode->state);

	for (auto i = 0; i < 4; ++i)	// @TODO: Hardcoded value of numOfOperators
	{
		Node* newNode = new Node(currentNode->cost + 1, currentNode->state);
		newNode->parent = currentNode;
// 		std::cout << "New node before inOperator" << i << std::endl;
// 		printGrid(newNode->state);

		inOperators[i](*this, newNode->state);

// 		std::cout << "\nNew node after inOperator" << i << std::endl;
// 		printGrid(newNode->state);
// 		std::cout << std::endl;

		if (!wasGridExplored(newNode->state))
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

int Search::calculateNumMisplacedTiles(gridi grid)
{
	// @TODO: This should simply compare to the goal grid
	int numMisplaced = 0;

	for (int i = 0; i < grid.size(); ++i)
	{
		for (int j = 0; j < grid[0].size(); ++j)
		{
			if (grid[i][j] == 0)
			{
				// Not counting blank piece as misplaced tile
				continue;
			}

			// Check explicitly if the last piece is not the 0 (blank) piece, others are straightforward
			if ((i == grid.size() - 1 && j == grid[0].size() - 1) ||
				grid[i][j] != (i * grid.size()) + j + 1)
			{
				++numMisplaced;
			}
		}
	}

	return numMisplaced;
}

void Search::aStarMisplacedTile(std::priority_queue < Node*, std::vector<Node*>, decltype(costComparisonLambda) >& outNodes, std::function<bool(Search&, gridi&)>* inOperators)
{
	if (outNodes.empty())
	{
		return;
	}

	Node* currentNode = outNodes.top();
	exploredGrids.push_back(currentNode->state);

	for (auto i = 0; i < 4; ++i)	// @TODO: Hardcoded value of numOfOperators
	{
		// Costs
		int g = currentNode->cost + 1;
		int h = calculateNumMisplacedTiles(currentNode->state);
		int finalCost = h + g;

		Node* newNode = new Node(finalCost, currentNode->state);
		newNode->parent = currentNode;
		// 		std::cout << "New node before inOperator" << i << std::endl;
		// 		printGrid(newNode->state, 3);

		inOperators[i](*this, newNode->state);

// 		std::cout << "\nNew node after inOperator" << i << std::endl;
// 		printGrid(newNode->state, 3);
// 		std::cout << std::endl;

		if (!wasGridExplored(newNode->state))
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
void Search::generalSearch(Problem problem, const std::function<void(Search&, std::priority_queue < Node*, std::vector<Node*>, decltype(costComparisonLambda) >&, std::function<bool(Search&, gridi&)>*)>& queueingFunction)
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

		if (isGoalState(currentNode->state, problem.goalState))
		{
			// @TODO: SUCCESS
			std::cout << "temp message: success\n";

			do
			{
				printGrid(currentNode->state);
				currentNode = currentNode->parent;
				std::cout << std::endl;
			} while (currentNode->parent);

			return;
		}

		queueingFunction(*this, nodes, problem.operators);
		nodes.pop();
	}
}

bool Search::isGoalState(const gridi& grid, const gridi& goal)
{
	return grid == goal;
}

bool Search::wasGridExplored(const gridi& grid)
{
	for (auto exploredGrid : exploredGrids)
	{
		if (grid == exploredGrid)
		{
			return true;
		}
	}

	return false;
}

void Search::printGrid(const gridi& grid)
{
	for (auto i = 0; i < grid.size(); ++i)
	{
		for (auto j = 0; j < grid[0].size(); ++j)
		{
			std::cout << grid[i][j] << '\t';
		}
		std::cout << std::endl;
	}
}

void Search::findZeroPiece(int& outRow, int& outCol, const gridi& grid)
{
	outRow = 0;
	outCol = 0;

	for (int i = 0; i < grid.size(); ++i)
	{
		for (int j = 0; j < grid[0].size(); ++j)
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

bool Search::movePieceDown(gridi& grid)
{
	// Find 0 "piece" (technically blank spot)
	int row = 0;
	int col = 0;

	findZeroPiece(row, col, grid);

	if (row == 0)
	{
		// Cannot move piece down into 0 spot if it is already at the topmost row
		return false;
	}

	std::swap(grid[row][col], grid[row - 1][col]);
	return true;
}

bool Search::movePieceUp(gridi& grid)
{
	// Find 0 "piece" (technically blank spot)
	int row = 0;
	int col = 0;

	findZeroPiece(row, col, grid);

	if (row == grid.size() - 1)
	{
		// Cannot move piece up into 0 spot if it is already at the bottommost row
		return false;
	}

	std::swap(grid[row][col], grid[row + 1][col]);
	return true;
}

bool Search::movePieceLeft(gridi& grid)
{
	// Find 0 "piece" (technically blank spot)
	int row = 0;
	int col = 0;

	findZeroPiece(row, col, grid);

	if (col == grid[0].size() - 1)
	{
		// Cannot move piece left into 0 spot if it is already at the rightmost column
		return false;
	}

	std::swap(grid[row][col], grid[row][col + 1]);
	return true;
}

bool Search::movePieceRight(gridi& grid)
{
	// Find 0 "piece" (technically blank spot)
	int row = 0;
	int col = 0;

	findZeroPiece(row, col, grid);

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
