#include <vector>

struct DPResult{
    long long totalValue;
    int totalWeight;
    std::vector<int> chosenItems;
};

//Knapsack
DPResult dpKnapsack(const std::vector<int>& weights, const std::vector<int>& values, int capacity);

//Maximum value
int knapsack(const std::vector<int>& weights, const std::vector<int>& values, int capacity);

//Which items were taken for the maximum value
std::vector<int> knapsackItems(const std::vector<int>& weights, const std::vector<int>& values, int capacity);

