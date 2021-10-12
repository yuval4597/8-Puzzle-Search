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
	Node* parent = nullptr;

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
auto costComparisonLambda = [](Node* a, Node* b) { return a->cost > b->cost; };

int** copyGrid(int** state, const int& n)
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

void printGrid(int** grid, const int& n)
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

std::vector<int**> exploredGrids;

bool wasGridExplored(int** grid, int n)
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

void uniformCostSearch(std::priority_queue < Node*, std::vector<Node*>, decltype(costComparisonLambda) >& outNodes, std::function<void(int**, int)>* inOperators)
{
	// @TODO: Keep track of explored nodes

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

		inOperators[i](newNode->state, 3);	// @TODO: Hardcoded value of n

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

bool isGoalState(int** grid, int** goal, const int& n)
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

// General search algorithm suggested by Dr. Keogh
void generalSearch(Problem problem, const std::function<void(std::priority_queue < Node*, std::vector<Node*>, decltype(costComparisonLambda) >&, std::function<void(int**, int)>*)>& queueingFunction)
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

        queueingFunction(nodes, problem.operators);
		nodes.pop();
    }
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
        {1,5,2},
        {0,7,4},
        {8,6,3}
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
	generalSearch(problem, uniformCostSearch);

	// @TODO: Deallocate all resources
}