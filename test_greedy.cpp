#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <stdexcept>
#include <limits>
#include "greedy.h"



int main() {
    std::cout << "Knapsack Greedy Algorithm" << std::endl;

    //I create a small example input. Each entry is {ID, weight, value}
    //I can replace these items later to test different cases
    std::vector<Item> items = {  {1, 2, 100}, {2, 3, 200}, {3, 1, 50}};
    //I set the maximum total weight that my bag can carry
    int capacity = 5;

    //I run my greedy function and keep its returned result in the answer
    GreedyResult answer = greedyKnapsack(items, capacity);

    
    //answer.selectedItems.push_back(1);
    
    
    //I print which item IDs were chosen and the final totals
    //This printing is outside the greedy function, so it does not need
    //to be included when I later measure the algorithm runtime
    std::cout << "Chosen Items: ";

    for (int ID : answer.chosenItems) {
        std::cout << ID << " ";
    }

    std::cout << "\nTotal Value: " << answer.totalValue << std::endl;
    std::cout << "Total Weight: " << answer.totalWeight << std::endl;
    std::cout << "Remaining Capacity: " << capacity - answer.totalWeight << std::endl;

    //I begin checking the result. First I make sure that the reported weight does not excced the bag capacity. This check alone
    //cannot prove that the reported total weight is correct
    if (answer.totalWeight > capacity) {
        std::cout << "Weight exceed the capacity" << std::endl;
    } else {
        std::cout << "Weight is within the capacity" << std::endl;
    }

     //I initially assume every chosen ID exists in the original inpit
     //if even on chosen ID is missing, I change validID to false
    bool validID = true;
    
    //I calculate the total value and weight again from the original items
    //This gives me another way to check the totals returned by greedyKanpsack
    long long calculatedValue = 0;
    long long calculatedWeight = 0;

    //I keep track of chosen IDs that I have already checkec
    std::unordered_set<int> appearedIDs;
    //I assume there are no duplicate chosen IDs until I find one
    bool notDuplicates = true;

    //I inspect each chosen ID, check for duplicated, then look it up
    //in the original input to calculate its real value and weight
    for (int selectedID : answer.chosenItems) {
        //I reset found for each new chosen ID. At this point, I have not yet found that particulat ID in the original items
        bool found = false; 

        //count() returns 1 if this selected ID is already counted in appearedIDs,
        //and 0 if it is not. Seeing it twice breaks the 0/1 rule
        if (appearedIDs.count(selectedID) > 0) {
            //I rememeber that at least one duploicated ID was detected
            notDuplicates = false;
            std::cout << "Duplicate item ID: " << selectedID << std::endl;
        } else {
            appearedIDs.insert(selectedID);
        }

        for (const Item& item : items) {
            if (item.ID == selectedID) {
                if (calculatedValue > std::numeric_limits<long long>::max() - item.value) {
                    throw std::overflow_error("Calculated value overflow");
                }
                calculatedValue += item.value;
                calculatedWeight += item.weight;
                //I found a matching item, so this chosen ID exists
                //I stop searching because input IDs are required to be unique
                found = true;
                break;
            }
        }
    
        //I use !found to mean "this chosen ID was not found"
        //if that condition is true, I mark the result as invalid
        if (!found) {
            validID = false;
            std::cout << "Invalid item ID: " << selectedID << std::endl;
        }
    }   

    //I compare the reported total weight with the weight I recalculated
    //This catches cases where the result reports a weight that does not
    //match the actual items chosen
    std::cout << "Calculated Weight: " << calculatedWeight << std::endl;   
    if (calculatedWeight == answer.totalWeight) {
        std::cout << "Total weight is correct" << std::endl;
    }  else {
        std::cout << "Total weight does not match" << std::endl;
    }

    //I do the same independent check for the total value
    if (calculatedValue == answer.totalValue) {
        std::cout << "Total value is correct" << std::endl;
    } else {
        std::cout << "Total value does not match" << std::endl;
    }       
    //I print the final ID checked only if no issue was detected
    if (validID) {
        std::cout << "All selected IDs exist" << std::endl;
    }
    if (notDuplicates) {
        std::cout << "No duplicate selected IDs" << std::endl;
    }
    return 0;
}
