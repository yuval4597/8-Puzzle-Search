#include "search.h"

#include <iostream>

Search::Search(gridi initialState, gridi goalState)
{
	std::vector<std::function<bool(Search&, gridi&)>> operatorList;
	operatorList.push_back(&Search::movePieceDown);
	operatorList.push_back(&Search::movePieceUp);
	operatorList.push_back(&Search::movePieceLeft);
	operatorList.push_back(&Search::movePieceRight);

	problem.initialState = initialState;
	problem.goalState = goalState;
	problem.operators = operatorList;
}

Search::~Search()
{
	deleteEncounteredNodes();
}

void Search::deleteEncounteredNodes()
{
	if (wereEncounteredNodesDeleted)
	{
		return;
	}

	numEncounteredNodes = encounteredNodes.size();

	for (auto i = 0; i < encounteredNodes.size(); ++i)
	{
		delete encounteredNodes[i];
		encounteredNodes[i] = nullptr;
	}

	encounteredNodes.resize(8);
	wereEncounteredNodesDeleted = true;
}

void Search::uniformCostSearch(std::priority_queue < Node*, std::vector<Node*>, decltype(costComparisonLambda) >& outNodes, std::vector<std::function<bool(Search&, gridi&)>> inOperators)
{
	if (outNodes.empty())
	{
		return;
	}

	Node* currentNode = outNodes.top();
	exploredGrids.insert(currentNode->state);

	for (auto i = 0; i < inOperators.size(); ++i)
	{
		// Here we can just add one to the totalCost when storing the newNode's cost since h(n) is always 0
		Node* newNode = new Node(currentNode->totalCost + 1, currentNode->state);
		newNode->parent = currentNode;

		inOperators[i](*this, newNode->state);

		if (wasGridExplored(newNode->state))
		{
			// Grid was already explored, can delete this new node
			delete newNode;
		}
		else
		{
			// Add to nodes and encounteredNodes (for deallocation later)
			outNodes.push(newNode);
			encounteredNodes.push_back(newNode);
		}
	}
}

int Search::calculateNumMisplacedTiles(const gridi& grid, const gridi& goal)
{
	int numMisplaced = 0;

	for (int i = 0; i < grid.size(); ++i)
	{
		for (int j = 0; j < grid[0].size(); ++j)
		{
			// Not counting blank piece as misplaced tile
			if (grid[i][j] == 0)
			{
				continue;
			}

			// Compare to goal state
			if (grid[i][j] != goal[i][j])
			{
				++numMisplaced;
			}
		}
	}

	return numMisplaced;
}

void Search::aStarMisplacedTile(std::priority_queue < Node*, std::vector<Node*>, decltype(costComparisonLambda) >& outNodes, std::vector<std::function<bool(Search&, gridi&)>> inOperators)
{
	if (outNodes.empty())
	{
		return;
	}

	Node* currentNode = outNodes.top();
	exploredGrids.insert(currentNode->state);

	for (auto i = 0; i < inOperators.size(); ++i)
	{
		Node* newNode = new Node(currentNode->state);
		newNode->parent = currentNode;

		inOperators[i](*this, newNode->state);

		if (wasGridExplored(newNode->state))
		{
			// Grid was already explored, can delete this new node
			delete newNode;
		}
		else
		{
			// Costs
			int g = currentNode->gCost + 1;
			int h = calculateNumMisplacedTiles(newNode->state, problem.goalState);
			int finalCost = h + g;

			newNode->gCost = g;
			newNode->totalCost = finalCost;

			// Add to nodes and encounteredNodes (for deallocation later)
			outNodes.push(newNode);
			encounteredNodes.push_back(newNode);
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
			// Ignoring 0 (blank) piece
			if (grid[i][j] == 0)
			{
				continue;
			}

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

int Search::getNumEncounteredNodes() const
{
	return numEncounteredNodes;
}

void Search::aStarManhattanDistance(std::priority_queue < Node*, std::vector<Node*>, decltype(costComparisonLambda) >& outNodes, std::vector<std::function<bool(Search&, gridi&)>> inOperators)
{
	if (outNodes.empty())
	{
		return;
	}

	Node* currentNode = outNodes.top();
	exploredGrids.insert(currentNode->state);

	for (auto i = 0; i < inOperators.size(); ++i)
	{
		Node* newNode = new Node(currentNode->state);
		newNode->parent = currentNode;

		inOperators[i](*this, newNode->state);

		if (wasGridExplored(newNode->state))
		{
			// Grid was already explored, can delete this new node
			delete newNode;
		}
		else
		{
			// Costs
			int g = currentNode->gCost + 1;
			int h = calculateManhattanDistance(newNode->state, problem.goalState);
			int finalCost = h + g;

			newNode->gCost = g;
			newNode->totalCost = finalCost;

			// Add to nodes and encounteredNodes (for deallocation later)
			outNodes.push(newNode);
			encounteredNodes.push_back(newNode);
		}
	}
}

// General search algorithm suggested by Dr. Keogh
bool Search::generalSearch(Problem problem, const std::function<void(Search&, std::priority_queue < Node*, std::vector<Node*>, decltype(costComparisonLambda) >&, std::vector<std::function<bool(Search&, gridi&)>>)>& queueingFunction)
{
	std::priority_queue<Node*, std::vector<Node*>, decltype(costComparisonLambda)> nodes(costComparisonLambda);

	// Initialize priority queue with initial state
	Node* initialNode = new Node(0, problem.initialState);
	nodes.push(initialNode);
	encounteredNodes.push_back(initialNode);

	// Keep looping until hit one of the return statements (or run out of memory :P)
	while (1)
	{
		if (nodes.empty())
		{
			std::cout << "FAILURE\n";

			deleteEncounteredNodes();
			return false;
		}

		Node* currentNode = nodes.top();

		if (isGoalState(currentNode->state, problem.goalState))
		{
			std::cout << "SUCCESS\n";
			int depth = 0;

			while (currentNode->parent)
			{
				++depth;
				printGrid(currentNode->state);
				currentNode = currentNode->parent;
				std::cout << std::endl;
			}

			std::cout << "Depth of " << depth << std::endl;

			deleteEncounteredNodes();
			return true;
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
