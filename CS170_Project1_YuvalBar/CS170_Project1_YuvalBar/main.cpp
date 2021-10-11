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
    int* operators;
};

struct Node
{
    int** state;
    int cost = 0;

    Node(int inCost) : cost(inCost) {}
};

// General search algorithm suggested by Dr. Keogh
void generalSearch(Problem problem, const std::function<void()>& queueingFunction)
{
    // Comparison lambda for prioritizing nodes
	auto costComparisonLambda = [](Node a, Node b) { return a.cost > b.cost; };
	std::priority_queue<Node, std::vector<Node>, decltype(costComparisonLambda)> nodes(costComparisonLambda);

    // Initialize priority queue with initial state
    nodes.push(problem.initialState);

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
}