#pragma once

#include <functional>
#include <vector>
#include <queue>

class Search;

// This struct holds all important information necessary to solve the problem
struct Problem
{
	int** initialState;
	int** goalState;
	std::function<bool(Search&, int**, const int&)>* operators;

	Problem() {}

	Problem(int** inInitialState, int** inGoalState, std::function<bool(Search&, int**, const int&)>* inOperators) :
		initialState(inInitialState),
		goalState(inGoalState),
		operators(inOperators)
	{}
};

// This struct holds information for each search node
struct Node
{
	int** state;
	int cost = 0;
	Node* parent = nullptr;

	Node(int inCost, int** inState) :
		cost(inCost),
		state(inState)
	{}
};

class Search
{
public:
	Search(int** initialState, int** goalState);

private:
	Problem problem;

	/* Search */
	// Comparison lambda for prioritizing nodes
	std::function<bool(Node*, Node*)> costComparisonLambda = [](Node* a, Node* b) { return a->cost > b->cost; };

	void uniformCostSearch(std::priority_queue < Node*, std::vector<Node*>, decltype(costComparisonLambda) >& outNodes, std::function<bool(Search&, int**, const int&)>* inOperators);

	int calculateNumMisplacedTiles(int** grid, const int& n);

	void aStarMisplacedTile(std::priority_queue < Node*, std::vector<Node*>, decltype(costComparisonLambda) >& outNodes, std::function<bool(Search&, int**, const int&)>* inOperators);

	void generalSearch(Problem problem, const std::function<void(Search&, std::priority_queue < Node*, std::vector<Node*>, decltype(costComparisonLambda) >&, std::function<bool(Search&, int**, const int&)>*)>& queueingFunction);

	/* Grid */
	bool isGoalState(int** grid, int** goal, const int& n);

	std::vector<int**> exploredGrids;

	bool wasGridExplored(int** grid, int n);

	void printGrid(int** grid, const int& n);
	
	int** copyGrid(int** state, const int& n);

	void findZeroPiece(int& outRow, int& outCol, int** grid, const int& n);

	/* Operators */
	// Function for moving piece down INTO the 0 spot
	// Returns false if the operation was unsuccessful
	bool movePieceDown(int** grid, const int& n);

	// Function for moving piece up INTO the 0 spot
	// Returns false if the operation was unsuccessful
	bool movePieceUp(int** grid, const int& n);

	// Function for moving piece left INTO the 0 spot
	// Returns false if the operation was unsuccessful
	bool movePieceLeft(int** grid, const int& n);

	// Function for moving piece right INTO the 0 spot
	// Returns false if the operation was unsuccessful
	bool movePieceRight(int** grid, const int& n);

public:
	void runUniformCostSearch();

	void runAstarMisplacedTileSearch();
};