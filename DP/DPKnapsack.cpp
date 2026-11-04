#include <vector>
#include <algorithm>
#include "DPKnapsack.hpp"

#include <iostream>

//Version that returns a DPResult (with total value, total weight and chosen items) - For benchmarking against the others which return the same things
DPResult dpKnapsack(const std::vector<int>& weights, const std::vector<long long>& values, int capacity) {
    if (weights.size() != values.size()) return {};

    std::vector<std::vector<int>> dp (weights.size() + 1, std::vector<int>(capacity + 1, 0));
    
    //First row and column are all zero
    //Row x -> x items
    //Column y -> y capacity

    //i = current number of items
    //c = current capacity
    for (std::size_t i = 1; i <= weights.size(); ++i)  {
        for (int c = 1; c <= capacity; ++c) {

            //Ignore this item -> value is the same as this capacity for the previous row
            int valueIgnoring = dp[i-1][c];

            //Take this item -> not possible if the item is too heavy
            int valueTaking = 0;

            //If the item can fit
            if (weights[i-1] <= c) {
                //add its value
                valueTaking += values[i-1];
                //Remaining value that can be taken is the amount that can be taken with the remaining weight
                valueTaking += dp[i-1][c-weights[i-1]];
            }

            //Pick the better value out of taking or ignoring the item
            int maxValue =  std::max(valueIgnoring, valueTaking);
            dp[i][c] = maxValue;

        }
    }

    std::vector<int> itemIndices (0);
    int remainingCapacity = capacity;
    int totalWeight = 0;

    for (int i = weights.size(); i > 0; --i) {
        if (dp[i][remainingCapacity] > dp[i-1][remainingCapacity]) {
            //This item was taken
            remainingCapacity -= weights[i-1];
            totalWeight += weights[i-1];
            itemIndices.push_back(i-1);
        }
        if (remainingCapacity <= 0) break;
    }

    std::reverse(itemIndices.begin(), itemIndices.end());

    return {dp[weights.size()][capacity], totalWeight, itemIndices};
}

//Knapsack - Take a list of weights and values, and a capacity: return just the maximum total value
int knapsack(const std::vector<int>& weights, const std::vector<long long>& values, int capacity) {

    if (weights.size() != values.size()) return -1;

    std::vector<int> currentRowDP (capacity + 1, 0);
    std::vector<int> previousRowDP (capacity + 1, 0); 
    
    //First row and column are all zero
    //Row x -> x items
    //Column y -> y capacity

    //i = current number of items
    //c = current capacity
    for (std::size_t i = 1; i <= weights.size(); ++i)  {
        for (int c = 1; c <= capacity; ++c) {

            //Ignore this item -> value is the same as this capacity for the previous row
            int valueIgnoring = previousRowDP[c];

            //Take this item -> not possible if the item is too heavy
            int valueTaking = 0;

            //If the item can fit
            if (weights[i-1] <= c) {
                //add its value
                valueTaking += values[i-1];
                //Remaining value that can be taken is the amount that can be taken with the remaining weight
                valueTaking += previousRowDP[c-weights[i-1]];
            }

            //Pick the better value out of taking or ignoring the item
            int maxValue =  std::max(valueIgnoring, valueTaking);
            currentRowDP[c] = maxValue;

        }
        previousRowDP = currentRowDP;
    }

    //The final cell is the maximum value
    return currentRowDP[capacity];
}

//Return the items that are taken for the maximum value
std::vector<int> knapsackItems(const std::vector<int>& weights, const std::vector<long long>& values, int capacity) {

    if (weights.size() != values.size()) return {};

    std::vector<std::vector<int>> dp (weights.size() + 1, std::vector<int>(capacity + 1, 0));
    
    //First row and column are all zero
    //Row x -> x items
    //Column y -> y capacity

    //i = current number of items
    //c = current capacity
    for (std::size_t i = 1; i <= weights.size(); ++i)  {
        for (int c = 1; c <= capacity; ++c) {

            //Ignore this item -> value is the same as this capacity for the previous row
            int valueIgnoring = dp[i-1][c];

            //Take this item -> not possible if the item is too heavy
            int valueTaking = 0;

            //If the item can fit
            if (weights[i-1] <= c) {
                //add its value
                valueTaking += values[i-1];
                //Remaining value that can be taken is the amount that can be taken with the remaining weight
                valueTaking += dp[i-1][c-weights[i-1]];
            }

            //Pick the better value out of taking or ignoring the item
            int maxValue =  std::max(valueIgnoring, valueTaking);
            dp[i][c] = maxValue;

        }
    }

    std::vector<int> itemIndices (0);
    int remainingCapacity = capacity;

    for (int i = weights.size(); i > 0; --i) {
        if (dp[i][remainingCapacity] > dp[i-1][remainingCapacity]) {
            //This item was taken
            remainingCapacity -= weights[i-1];
            itemIndices.push_back(i-1);
        }
        if (remainingCapacity <= 0) break;
    }

    std::reverse(itemIndices.begin(), itemIndices.end());

    return itemIndices;
}