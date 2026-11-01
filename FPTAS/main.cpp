// FPTAS-Knapsack-Algorithm.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "FPTAS.hpp"

int main() {
    std::vector<Item> items = {
        {4, 10},
        {8, 25},
        {12, 40},
        {3, 15},
        {5, 20}
    };

    int capacity = 15;
    double epsilon = 0.2;

    std::vector<int> result = knapsackFPTAS(items, capacity, epsilon);

    int totalWeight = 0;
    int totalValue = 0;

    for (int index : result) {
        totalWeight += items[index].weight;
        totalValue += items[index].value;
    }

    std::cout << "Selected items: ";

    for (int index : result) {
        std::cout << index << " ";
    }

    std::cout << "\nTotal weight: " << totalWeight;
    std::cout << "\nTotal value: " << totalValue << std::endl;

    return 0;
}