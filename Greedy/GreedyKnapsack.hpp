#ifndef GREEDY_H
#define GREEDY_H

#include <vector>


//I use struct to keep the information for one item
//Each item has its ID, weight, value
struct Item {
    //I use this ID to identify the item when checking the selected result
    int ID; 
    //I store the weight of the item, not the capacity of the bag
    int weight; 
    //I store the valuable of this item. I use long long for a larger values
    long long value;
};
//I use this struct to return all the information from my greedy algorithm
//This also makes it easier to check whether the selected items form a valid solution
struct GreedyResult{
    long long totalValue;
    int totalWeight;
    std::vector<int> chosenItems;
};

//I declare my function here so other files can use it
GreedyResult greedyKnapsack(const std::vector<Item>& items, int capacity);

#endif