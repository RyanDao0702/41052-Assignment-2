#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <stdexcept>
#include <limits>
#include "GreedyKnapsack.hpp"



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



    //I test what happens when there are no items
    //Greedy should return 0 values, 0 weights
    //and an empty list of chosen items
    std::vector<Item> emptyBag;
    GreedyResult emptyResult = greedyKnapsack(emptyBag, 10);

    if (emptyResult.totalValue == 0 && emptyResult.totalWeight == 0 && emptyResult.chosenItems.empty()) {
        std::cout << "Empty input test: Pass" << std::endl;
    } else {
        std::cout << "Empty input test: Fail" << std::endl;
        return 1;
    }
    //Now I want to test what happens when the capacity is 0
    //Although there are items, but there is nothing fit
    //I expect the total value and weight to be 0
    //The list of chosen items should also be empty
    std::vector<Item> noCapacityItems = {{1,2,100}, {2,3,200}, {3,1,50}};

    GreedyResult noResult = greedyKnapsack(noCapacityItems, 0);

    if (noResult.totalValue == 0 && noResult.totalWeight == 0 && noResult.chosenItems.empty()) {
        std::cout << "No capacity test: Pass" << std::endl;
    } else {
        std::cout << "No capacity test: Fail" << std::endl;
        return 1;
    }

    //I want to check that my Greedy function does not accept an item
    //whoose weight is 0. In my implementation, every item must have a weight > 0 
    //The first item below has weight 0 on purpose. The second item is normal, so I can check that one bad item is enough to reject 
    //the input
    std::vector<Item> noWeightItems = {{1,0,100}, {2,3,200}};

    //I initialise with false since Greedy has not rejected the input yet
    //I will change this to true only if Greedy reports the error
    //that I expect
    bool noWeightRejected = false;

    //I put the call inside try because I expect it may throw an error
    //If the input is rejected, Greedy uses throw to report the problem
    //When that happens, normal execution of the try block stops,
    //and C++ looks for a matching catch block below
    try {
        //I intentionally give Greedy an invalid item (Weight 0)
        //The capacity is 5, but capacity is not the problem here
        //This test is only checking whether invalid weight is rejected
        greedyKnapsack(noWeightItems, 5);
    } catch (const std::invalid_argument&) {
        //This catch runs only when an std::invalid_argument error
        //is thrown from the try block
        //I do not give the error a name because I do not
        //need to read its message in this test
        //Reaching this line means Greedy rejected the bad input
        //using the error type I expected
        noWeightRejected = true; 
    }

    //If Greedy threw the correct error, the variable is now true
    //I print Pass because the input validation worked as expected
    if (noWeightRejected) {
        std::cout << "No weight test: Pass" << std::endl;
    } else {
        //If the variable is still false, Greedy did not throw
        //std::invalid_argument as expected, so this test fails
        std::cout << "No weight test: Fail" << std::endl;
        //I return 1 to stop this test and report the failure
        //Returning 0 would normally tell the system the program 
        //finished successfully, so I do not use return 0 when a test fails
        return 1;
    }

    //I test items that all have the same value density
    //For instance, 
    //Item 1: 20/2 = 10
    //Item 2: 30/3=10
    //Item 3: 40/4=10
    //Greedy should consider the item with the smaller ID first
    //For example, with the capacity 5, I expect items 1 and 2 to be chosen
    std::vector<Item> equalDensityItems = {{1,2,20}, {2,3,30}, {3,4,40}};

    //I use capacity capacity 5 for this for this test
    //Greedy should first choose item 1 (weight 2, value 20)
    //The remaining capacity is then 3
    //It should next choose Item 2 (weight 3, value 30)
    //The remaining capacity becomes 0, so Item 3 cannot fit
    GreedyResult equalResult = greedyKnapsack(equalDensityItems, 5);

    //I check all three expected results, not just the total value
    //totalValue must be 20 + 30 = 50
    //totalWeight must be 2 + 3 =5
    //chosenItems must contain item 1 followed by item 2
    //std::vector<int>{1,2} is the exact list I expect
    //IDs in the same order 
    if (equalResult.totalValue == 50 && equalResult.totalWeight == 5 && equalResult.chosenItems == std::vector<int>{1,2}) {
        std::cout << "Equal density test: Pass" << std::endl;
    } else {
        std::cout << "Equal density test: Fail" << std::endl;
        return 1;
    }

    
    // I test a case where Greedy does not find the best answer.
    // Item 1 has a higher value density, so Greedy picks it first.
    // But Item 2 alone gives a much higher total value.
    // This is a limitation of Greedy
    std::vector<Item> hardItems = {{1, 1, 2}, {2, 10, 19}};

    GreedyResult hardAnswer = greedyKnapsack(hardItems, 10);

    if (hardAnswer.totalValue == 2 && hardAnswer.totalWeight == 1 && hardAnswer.chosenItems == std::vector<int>{1}) {
        std::cout << "Greedy limitation test: Pass" << std::endl;
    } else {
        std::cout << "Greedy limitation test: Fail" << std::endl;
        return 1;
    }
    return 0;
}
