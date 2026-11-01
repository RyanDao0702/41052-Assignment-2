#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <stdexcept>
#include <limits>
#include "greedy.h"

//I put the greedy algorithm in its own function so I can call it using 
//different test data and compare it with other two algorithms in my team
//I pass items by const reference to avoid copying the input just to call the function
GreedyResult greedyKnapsack(const std::vector<Item>& items, int capacity) {
    //I first check whether the bag capacity is valid since negative capacity
    //does not make sense for the input rules used in this implementation
    if (capacity < 0) {
        throw std::invalid_argument("Capacity cannot be negative");
    }
    //I require positive item weights, because the density comparison later
    //divided values by weight conceptually, and assumes positive weights
    for (const Item& item : items) {
        if (item.weight <= 0 ) {
            throw std::invalid_argument("Item weight must be positive");
        }
        if (item.value < 0) {
            throw std::invalid_argument("Item value cannot be negative");
        }
    }   

    //I use a set to check that each input item has a unique ID
    //If two different items have the same ID, my result checker cannot
    //reliably identify which one the algorithm selected
    std::unordered_set<int> inputIDs;

    for(const Item& item : items) {
        //count() checks whether this ID was already there. If not, add it
        if (inputIDs.count(item.ID) > 0 ) {
            throw std::invalid_argument("Duplicate item ID");
        }
        //I add the ID only after confirming that it is not already present
        inputIDs.insert(item.ID);
    }
    
    //I make a copy of the iotems because sorting changes their order
    //I want to sort this copy while keeping the original input unchanged
    std::vector<Item> sortedItems = items;
    
    //I start with no items chosen, so both totals are 0
    //and the chosen item list is empty
    GreedyResult result{0, 0, {}};

    //I sort items from highest to lowest 
    //Instead of dividing, I compare the fractions by cross multiplication
    //to avoid floating-point rounding when the densities are very close
    //I use the __int128 extension to avoid overflowing these products
    //When two densities are equal, I put the smaller item ID first
    //so the sorting rule gives a consistent result
    std::sort(sortedItems.begin(), sortedItems.end(),[](const Item& a, const Item& b) {
        __int128 left = static_cast<__int128>(a.value) * b.weight;
        __int128 right = static_cast<__int128>(b.value) * a.weight;

        if (left != right) {
            return left > right;
        }
        return a.ID < b.ID;
    });

    //I now go through the items in the order decided above
    //This is a 0/1 greedy choise: I either take the whole item once
    //or skip it if it cannot fit
    for (const Item& item : sortedItems) {
        //I compare the item weight with the remaining bag capacity
        //subtracting the weight already selected avoids adding 2 weights
        //in the condition, which could overflow with large integers
        if (item.weight <= capacity - result.totalWeight) {
            //I update the weight after deciding that this item fits
            result.totalWeight += item.weight;
            //I check the value before adding it to avoid signed overflow
            //This check is reached only for items that fit in the bag
            if (item.value < 0 || result.totalValue > std::numeric_limits<long long>::max() - item.value) {
                throw std::overflow_error("Invalid or overflowing total value");
            }
            //I add the item value and remember its ID in the answer
            result.totalValue += item.value;
            result.chosenItems.push_back(item.ID);
        }
    }

    //I return the three results rather than printing
    return result;
}
