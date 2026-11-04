#include "DP\DPKnapsack.hpp"
#include "FPTAS\FPTAS.hpp"
#include "Greedy\GreedyKnapsack.hpp"
#include <iostream>
#include <vector>

template <typename T>
std::string vectorToString(const std::vector<T>& vec) {
    std::string result = "{";

    for (std::size_t i = 0; i < vec.size(); ++i) {
        if (i > 0) result += ", ";
        result += std::to_string(vec[i]);
    }

    result += "}";
    return result;
}


void testKnapsack(
    const std::vector<int>& weights,
    const std::vector<long long>& values,
    int capacity,
    int expected) 
{

    std::vector<Item> items;
    std::vector<FPTASItem> fptasItems;

    for (std::size_t i = 0; i < weights.size(); ++i ) {
        items.push_back(Item{i, weights[i], values[i]});
        fptasItems.push_back({ weights[i], static_cast<int>(values[i]) });
    }

    DPResult dpresult = dpKnapsack(weights, values, capacity);
    GreedyResult greedyResult = greedyKnapsack(items, capacity);
    std::vector<int> FPTASTaken = knapsackFPTAS(fptasItems, capacity, 0.2);
    int FPTASResult = 0;
    for (int i = 0; i < FPTASTaken.size(); ++i) {
        FPTASResult += fptasItems[FPTASTaken[i]].value;
    }

    std::cout << "Knapsack Test\n Weights " << vectorToString(weights) << " Values " << vectorToString(values) << " Capacity: " << capacity << '\n';
    std::cout << "Expected: " << expected << '\n';


    std::cout << "DP: " << dpresult.totalValue << '\n';
    std::cout << "FPTAS: " << FPTASResult << '\n';
    std::cout << "Greedy: " << greedyResult.totalValue << '\n';

}


int main() {

    // testKnapsack({}, {}, 10, 0);

    // testKnapsack({2, 3, 4}, {3, 4, 5}, 5, 7);

    // testKnapsack({5}, {10}, 4, 0);

    // testKnapsack({5}, {10}, 5, 10);

    // testKnapsack({1, 1, 1}, {2, 3, 4}, 2, 7);

    // testKnapsack({2, 3}, {0, 0}, 5, 0);

    testKnapsack({51, 50, 50}, {102, 99, 99}, 100, 198);

    return 0;
}