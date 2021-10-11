// Yuval Bar 2021

#include <iostream>
#include <functional>
#include <vector>
#include <queue>

// This struct holds all important information necessary to solve the problem
struct Problem
{
    int** initialState;
    int** goalState;
    std::function<void(int**, int)>* operators;

    Problem(int** inInitialState, int** inGoalState, std::function<void(int**, int)>* inOperators) :
        initialState(inInitialState),
        goalState(inGoalState),
        operators(inOperators)
    {}
};

struct Node
{
    int** state;
    int cost = 0;

    Node(int inCost, int** inState) : 
        cost(inCost),
        state(inState)
    {}
};

void findZeroPiece(int& outRow, int& outCol, int** grid, const int& n)
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

// Function for moving piece down INTO the 0 spot
// Returns false if the operation was unsuccessful
bool movePieceDown(int** grid, const int& n)
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

// Function for moving piece up INTO the 0 spot
// Returns false if the operation was unsuccessful
bool movePieceUp(int** grid, const int& n)
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

// Function for moving piece left INTO the 0 spot
// Returns false if the operation was unsuccessful
bool movePieceLeft(int** grid, const int& n)
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

// Function for moving piece right INTO the 0 spot
// Returns false if the operation was unsuccessful
bool movePieceRight(int** grid, const int& n)
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

// Comparison lambda for prioritizing nodes
auto costComparisonLambda = [](Node a, Node b) { return a.cost > b.cost; };

// General search algorithm suggested by Dr. Keogh
void generalSearch(Problem problem, const std::function<void(std::priority_queue < Node, std::vector<Node>, decltype(costComparisonLambda) >&)>& queueingFunction)
{
	std::priority_queue<Node, std::vector<Node>, decltype(costComparisonLambda)> nodes(costComparisonLambda);

    // Initialize priority queue with initial state
    const Node initialNode(0, problem.initialState);
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

        Node currentNode = nodes.top();
        nodes.pop();

        if (currentNode.state == problem.goalState)
        {
            // @TODO: SUCCESS
            std::cout << "temp message: success\n";
            return;
        }

        queueingFunction(nodes);
    }
}

void pushQueue(std::priority_queue < Node, std::vector<Node>, decltype(costComparisonLambda) >& pq)
{
	pq.push(Node(2, nullptr));
	pq.push(Node(4, nullptr));
	pq.push(Node(3, nullptr));
}

void printQueue(std::priority_queue < Node, std::vector<Node>, decltype(costComparisonLambda) > pq)
{
	while (!pq.empty())
	{
		std::cout << pq.top().cost;
		pq.pop();
	}
}

void printGrid(int** grid, const int& n)
{

}

int main()
{
//     int** grid;
//     int rows, cols;
// 
//     std::cout << "Rows: ";
//     std::cin >> rows;
//     std::cout << "Cols: ";
//     std::cin >> cols;
// 
//     grid = new int*[rows];
//     for (auto i = 0; i < rows; ++i)
//     {
//         grid[i] = new int[cols];
//     }
// 
//     for (int i = 0; i < rows; ++i)
//     {
//         for (int j = 0; j < cols; ++j)
//         {
//             grid[i][j] = 0;
//         }
//     }
// 
// 	for (int i = 0; i < rows; ++i)
// 	{
// 		for (int j = 0; j < cols; ++j)
// 		{
//             std::cout << grid[i][j];
// 		}
//         std::cout << std::endl;
// 	}
// 
// 	for (auto i = 0; i < rows; ++i)
// 	{
// 		delete[] grid[i];
// 	}
// 
//     delete[] grid;
// 
//     std::cout << "All done!\n";
//     std::cout << "Over.\n";

    // n is the number of rows/columns (must be a square grid)
    const int n = 3;

	int** solvedGrid = new int* [n];
	for (auto i = 0; i < n; ++i)
	{
		solvedGrid[i] = new int[n];
	}

    std::cout << "Solved grid:\n";

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
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

    int initialGrid[][3] = {
        {6,3,7},
        {4,8,5},
        {1,2,0}
    };

	int** initialGridPtr = new int* [n];
	for (auto i = 0; i < n; ++i)
	{
        initialGridPtr[i] = new int[n];
	}

	std::cout << "initial grid ptr:\n";

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
            initialGridPtr[i][j] = initialGrid[i][j];
			std::cout << initialGridPtr[i][j] << '\t';
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
	
	const int numOfOperators = 4;	// Up, down, left, right
	std::function<void(int**, int)>* operatorList = new std::function<void(int**, int)>[numOfOperators];
	operatorList[0] = movePieceDown;
	operatorList[1] = movePieceUp;
	operatorList[2] = movePieceLeft;
	operatorList[3] = movePieceRight;

	Problem problem(initialGridPtr, solvedGrid, operatorList);
	// generalSearch with queueing function that takes operators...

	// @TODO: Deallocate all resources
}