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
	exploredGrids.insert(currentNode->state);

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
}

int Search::calculateNumMisplacedTiles(const gridi& grid)
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
	exploredGrids.insert(currentNode->state);

	for (auto i = 0; i < 4; ++i)	// @TODO: Hardcoded value of numOfOperators
	{
		Node* newNode = new Node(currentNode->state);
		newNode->parent = currentNode;
		// 		std::cout << "New node before inOperator" << i << std::endl;
		// 		printGrid(newNode->state, 3);

		inOperators[i](*this, newNode->state);

// 		std::cout << "\nNew node after inOperator" << i << std::endl;
// 		printGrid(newNode->state, 3);
// 		std::cout << std::endl;

		// Costs
		int g = currentNode->cost + 1;
		int h = calculateNumMisplacedTiles(newNode->state);
		int finalCost = h + g;

		newNode->cost = finalCost;

		if (!wasGridExplored(newNode->state))
		{
			outNodes.push(newNode);
		}
	}
}

int Search::calculateManhattanDistance(const gridi& grid, const gridi& goal)
{
	int totalDistance = 0;

	for (auto i = 0; i < grid.size(); ++i)
	{
		for (auto j = 0; j < grid[0].size(); ++j)
		{
			int row = 0;
			int col = 0;

			// Find this piece in the goal grid in order to calculate distance
			findPiece(grid[i][j], row, col, goal);

			const int distanceForThisTile = abs(row - i) + abs(col - j);
			totalDistance += distanceForThisTile;
		}
	}

	return totalDistance;
}

void Search::aStarManhattanDistance(std::priority_queue < Node*, std::vector<Node*>, decltype(costComparisonLambda) >& outNodes, std::function<bool(Search&, gridi&)>* inOperators)
{
	if (outNodes.empty())
	{
		return;
	}

	Node* currentNode = outNodes.top();
	exploredGrids.insert(currentNode->state);

	for (auto i = 0; i < 4; ++i)	// @TODO: Hardcoded value of numOfOperators
	{
		Node* newNode = new Node(currentNode->state);
		newNode->parent = currentNode;
		// 		std::cout << "New node before inOperator" << i << std::endl;
		// 		printGrid(newNode->state, 3);

		inOperators[i](*this, newNode->state);

		// 		std::cout << "\nNew node after inOperator" << i << std::endl;
		// 		printGrid(newNode->state, 3);
		// 		std::cout << std::endl;

		// Costs
		int g = currentNode->cost + 1;
		int h = calculateManhattanDistance(newNode->state, problem.goalState);
		int finalCost = h + g;

		newNode->cost = finalCost;

		if (!wasGridExplored(newNode->state))
		{
			outNodes.push(newNode);
		}

		// @TODO : Else delete unused new node (do in all queuing functions?)
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
			int depth = 0;

			while (currentNode->parent)
			{
				++depth;
				printGrid(currentNode->state);
				currentNode = currentNode->parent;
				std::cout << std::endl;
			}

			std::cout << "Depth of " << depth << std::endl;
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
	auto iter = exploredGrids.find(grid);
	if (iter != exploredGrids.end())
	{
		return true;
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

void Search::findPiece(int piece, int& outRow, int& outCol, const gridi& grid)
{
	outRow = 0;
	outCol = 0;

	for (int i = 0; i < grid.size(); ++i)
	{
		for (int j = 0; j < grid[0].size(); ++j)
		{
			if (grid[i][j] == piece)
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

	findPiece(0, row, col, grid);

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

	findPiece(0, row, col, grid);

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

	findPiece(0, row, col, grid);

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

	findPiece(0, row, col, grid);

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

void Search::runAstarManhattanDistanceSearch()
{
	generalSearch(problem, &Search::aStarManhattanDistance);
}
