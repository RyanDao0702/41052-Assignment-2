#include "DPKnapsack.hpp"
#include <iostream>
#include <vector>

std::string vectorToString(const std::vector<int>& vec) {
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
    const std::vector<int>& values,
    int capacity,
    int expected
) {
    DPResult result = dpKnapsack(weights, values, capacity);


    std::cout << "Knapsack Test\n Weights " << vectorToString(weights) << " Values " << vectorToString(values) << " Capacity: " << capacity << '\n';
    std::cout << "Expected: " << expected << ". Got: " << result.totalValue << '\n';
    std::cout << "Took items: " << vectorToString(result.chosenItems) << '\n';

}


int main() {

    // testKnapsack({}, {}, 10, 0);

    // testKnapsack({2, 3, 4}, {3, 4, 5}, 5, 7);

    // testKnapsack({5}, {10}, 4, 0);

    // testKnapsack({5}, {10}, 5, 10);

    // testKnapsack({1, 1, 1}, {2, 3, 4}, 2, 7);

    // testKnapsack({2, 3}, {0, 0}, 5, 0);

    testKnapsack({51, 50, 50}, {102, 99, 99}, 10, 0);

    return 0;
}