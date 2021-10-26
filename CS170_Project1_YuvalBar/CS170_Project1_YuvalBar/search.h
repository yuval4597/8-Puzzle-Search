#pragma once

#include <functional>
#include <vector>
#include <queue>
#include <unordered_set>

// Defining gridi as 2D array of ints for simplicity
using gridi = std::vector<std::vector<int>>;

// Forward declaration for Problem struct
class Search;

// This struct holds all important information necessary to solve the problem
struct Problem
{
	gridi initialState;
	gridi goalState;
	std::function<bool(Search&, gridi&)>* operators;

	Problem() {}

	Problem(gridi inInitialState, gridi inGoalState, std::function<bool(Search&, gridi&)>* inOperators) :
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

	Node(gridi inState) :
		cost(0),
		state(inState)
	{}
};

struct HashFn
{
	size_t operator()(const gridi& g) const
	{
		// Primes
		const int p = 7919;
		const int q = 3671;

		size_t res = 0;
		for (size_t i = 0; i < g.size(); ++i)
		{
			for (size_t j = 0; j < g[0].size(); ++j)
			{
				res ^= (static_cast<int>(pow(p, i)) + static_cast<int>(pow(q, j))) * g[i][j];
			}
		}

		return res;
	}
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

	void uniformCostSearch(std::priority_queue < Node*, std::vector<Node*>, decltype(costComparisonLambda) >& outNodes, std::function<bool(Search&, gridi&)>* inOperators);

	int calculateNumMisplacedTiles(const gridi& grid);

	void aStarMisplacedTile(std::priority_queue < Node*, std::vector<Node*>, decltype(costComparisonLambda) >& outNodes, std::function<bool(Search&, gridi&)>* inOperators);

	int calculateManhattanDistance(const gridi& grid, const gridi& goal);

	void aStarManhattanDistance(std::priority_queue < Node*, std::vector<Node*>, decltype(costComparisonLambda) >& outNodes, std::function<bool(Search&, gridi&)>* inOperators);

	void generalSearch(Problem problem, const std::function<void(Search&, std::priority_queue < Node*, std::vector<Node*>, decltype(costComparisonLambda) >&, std::function<bool(Search&, gridi&)>*)>& queueingFunction);

	/* Grid */
	bool isGoalState(const gridi& grid, const gridi& goal);

	std::unordered_set<gridi, HashFn> exploredGrids;

	bool wasGridExplored(const gridi& grid);

	void printGrid(const gridi& grid);

	void findPiece(int piece, int& outRow, int& outCol, const gridi& grid);

	/* Operators */
	// Function for moving piece down INTO the 0 spot
	// Returns false if the operation was unsuccessful
	bool movePieceDown(gridi& grid);

	// Function for moving piece up INTO the 0 spot
	// Returns false if the operation was unsuccessful
	bool movePieceUp(gridi& grid);

	// Function for moving piece left INTO the 0 spot
	// Returns false if the operation was unsuccessful
	bool movePieceLeft(gridi& grid);

	// Function for moving piece right INTO the 0 spot
	// Returns false if the operation was unsuccessful
	bool movePieceRight(gridi& grid);

public:
	void runUniformCostSearch();

	void runAstarMisplacedTileSearch();

	void runAstarManhattanDistanceSearch();
};