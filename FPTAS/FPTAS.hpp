#pragma once

#ifndef FPTAS_HPP
#define FPTAS_HPP

#include <vector>

// Represents an item in the knapsack problem.
struct FTPASItem {
    int weight;
    int value;
};

// Finds an approximate solution using the FPTAS algorithm.
std::vector<int> knapsackFPTAS(
    const std::vector<FPTASItem>& items,
    int capacity,
    double epsilon
);

#endif