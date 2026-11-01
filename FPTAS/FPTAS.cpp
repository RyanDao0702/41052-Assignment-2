#include "FPTAS.hpp"
#include <cmath>
#include <climits>

std::vector<int> knapsackFPTAS(const std::vector<Item>& items, int capacity, double epsilon) {

    // Check that the input parameters are valid.
    if (epsilon <= 0 || epsilon >= 1 || capacity < 0) {
        return {};
    }

    int maxValue = 0; //initialise the start value

    for (int i = 0; i < items.size(); i++) { //check each item
        if (items[i].weight <= capacity) { //ignore if the item cant fit
            if (items[i].value > maxValue) {
                maxValue = items[i].value; //update largest value
            }
        }
    }

    if (maxValue == 0) { //return nothing if no max value
        return {};
    }

    double K = epsilon * maxValue / items.size(); // scaling factor

    std::vector<int> scaledValues(items.size(), 0); //scale values

    for (int i = 0; i < items.size(); i++) { //for each item
        if (items[i].weight <= capacity && items[i].value > 0) {
            scaledValues[i] = static_cast<int>( //use scaling factor so that the DP doesn't need to handle the full original values
                std::floor(items[i].value / K) //round down to the nearest integer
                );
        }
    }

    int totalScaledValue = 0; //initialise total scaled value

    for (int i = 0; i < items.size(); i++) {
        if (items[i].weight <= capacity) { //calculate the largest possible sum of scaled values if every item is included individually
            totalScaledValue += scaledValues[i];
        }
    }

    //std::vector<int> dp(totalScaledValue + 1, INT_MAX); //dp to store minimum weight needed for each scaled value where INT_MAX means the value is unreachable
    //dp[0] = 0; //set DP to zero because weight 0 if none selected

    //for (int i = 0; i < items.size(); i++) { //process each item
    //    if (items[i].weight > capacity) {
    //        continue;
    //    }

    //    for (int v = totalScaledValue; v >= scaledValues[i]; v--) { //loop backwards to prevent the same item being used in the same iteration
    //        if (dp[v - scaledValues[i]] != INT_MAX) {
    //            int newWeight = dp[v - scaledValues[i]] + items[i].weight; //calculate new weight

    //            if (newWeight < dp[v]) {  //check if new weight is new best
    //                dp[v] = newWeight; //update dp
    //            }
    //        }
    //    }
    //}

    std::vector<std::vector<int>> dp(items.size() + 1, std::vector<int>(totalScaledValue + 1, INT_MAX)); //create 2d dp table to store minimum weight required to achieve a particular scaled value
    //use item.size() + 1 because row 0 will be when no items are considered similarly totalScaledValue + 1 needs to represent every scaled value including 0
    dp[0][0] = 0; //set initial value to 0 when 0 items are considered

    for (int i = 1; i <= items.size(); i++) { //loop through each item for processing
        int weight = items[i - 1].weight; //use i - 1 because dp table starts at row 0 but the item vector starts at index 0
        int value = scaledValues[i - 1]; //store the wegiht and value in variables  

        // Ignore items that cannot fit individually.
        if (weight > capacity) {
            for (int v = 0; v <= totalScaledValue; v++) {
                dp[i][v] = dp[i - 1][v];
            }
            continue;
        }

        for (int v = 0; v <= totalScaledValue; v++) { //check every possible scaled value 
            // Don't select this item
            dp[i][v] = dp[i - 1][v]; //copy the result from the previous row if the next item isn't selected

            // Select this item
            if (v >= value && dp[i - 1][v - value] != INT_MAX) { //the target scaled value needs to be at least as big as the current item's scaled value and we need to already have a way to achieve the remaining scaled value

                int newWeight = dp[i - 1][v - value] + weight; //calculate the new weight required if the new item is included

                if (newWeight < dp[i][v]) { //check if the new weight is less, if it is then we update the dp table
                    dp[i][v] = newWeight;
                }
            }
        }
    }

    int bestValue = 0;

    for (int v = 0; v <= totalScaledValue; v++) { //search through the dp table and find the highest scaled value that doesn't exceed the capacity 
        if (dp[items.size()][v] <= capacity) {
            bestValue = v; //save the value into best value
        }
    }

    std::vector<int> selectedItems;

    int v = bestValue;

    for (int i = items.size(); i > 0; i--) { //loop backwards through the items to retrace each scaled value to the original value

        if (dp[i][v] != dp[i - 1][v]) { //if the values are not the same then it was an item that was selected 

            selectedItems.push_back(i - 1); //add to the vector

            v -= scaledValues[i - 1]; //remove the scaled value 
        }
    }

    return selectedItems;
}