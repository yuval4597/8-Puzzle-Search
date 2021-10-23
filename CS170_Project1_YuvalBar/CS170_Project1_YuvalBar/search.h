#pragma once

#include <functional>
#include <vector>
#include <queue>

// Defining gridi as 2D array of ints for simplicity
using gridi = int**;

// Forward declaration for Problem struct
class Search;

// This struct holds all important information necessary to solve the problem
struct Problem
{
	gridi initialState;
	gridi goalState;
	std::function<bool(Search&, gridi, const int&)>* operators;

	Problem() {}

	Problem(gridi inInitialState, gridi inGoalState, std::function<bool(Search&, gridi, const int&)>* inOperators) :
		initialState(inInitialState),
		goalState(inGoalState),
		operators(inOperators)
	{}
};

// This struct holds information for each search node
struct Node
{
	gridi state;
	int cost = 0;
	Node* parent = nullptr;

	Node(int inCost, gridi inState) :
		cost(inCost),
		state(inState)
	{}
};

class Search
{
public:
	Search(gridi initialState, gridi goalState);

private:
	Problem problem;

	/* Search */
	// Comparison lambda for prioritizing nodes
	std::function<bool(Node*, Node*)> costComparisonLambda = [](Node* a, Node* b) { return a->cost > b->cost; };

	void uniformCostSearch(std::priority_queue < Node*, std::vector<Node*>, decltype(costComparisonLambda) >& outNodes, std::function<bool(Search&, gridi, const int&)>* inOperators);

	int calculateNumMisplacedTiles(gridi grid, const int& n);

	void aStarMisplacedTile(std::priority_queue < Node*, std::vector<Node*>, decltype(costComparisonLambda) >& outNodes, std::function<bool(Search&, gridi, const int&)>* inOperators);

	void generalSearch(Problem problem, const std::function<void(Search&, std::priority_queue < Node*, std::vector<Node*>, decltype(costComparisonLambda) >&, std::function<bool(Search&, gridi, const int&)>*)>& queueingFunction);

	/* Grid */
	bool isGoalState(gridi grid, gridi goal, const int& n);

	std::vector<gridi> exploredGrids;

	bool wasGridExplored(gridi grid, int n);

	void printGrid(gridi grid, const int& n);
	
	gridi copyGrid(gridi state, const int& n);

	void findZeroPiece(int& outRow, int& outCol, gridi grid, const int& n);

	/* Operators */
	// Function for moving piece down INTO the 0 spot
	// Returns false if the operation was unsuccessful
	bool movePieceDown(gridi grid, const int& n);

	// Function for moving piece up INTO the 0 spot
	// Returns false if the operation was unsuccessful
	bool movePieceUp(gridi grid, const int& n);

	// Function for moving piece left INTO the 0 spot
	// Returns false if the operation was unsuccessful
	bool movePieceLeft(gridi grid, const int& n);

	// Function for moving piece right INTO the 0 spot
	// Returns false if the operation was unsuccessful
	bool movePieceRight(gridi grid, const int& n);

public:
	void runUniformCostSearch();

	void runAstarMisplacedTileSearch();
};