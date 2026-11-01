#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include "GreedyKnapsack.hpp"
#include <random>

//This is my own test function. It does NOT change the Greedy Algorithm
//I use it to see if the answer returned by Greedy follows the knapsack rules
//I return true when everything looks correct, or false if I find a problem
bool checkAnswer(const std::vector<Item>& items, int capacity, const GreedyResult& answer) {
    
    //I start my own totals at zero because I have not checked any chosen items yet
    //I will calculate the totals from the original input instead of trusting Greedy's total
    long long totalValue = 0;
    long long totalWeight = 0;

    //This set remembers which item IDs I have already seen
    //an ID should appear at most once because this is the 0/1 Knapsack problem
    std::unordered_set<int> chosenIDs;

    //I look at every ID that Greedy says it selected
    for (int pickedID : answer.chosenItems) {
        //count() is 1 if this ID is already in thr set, and 0 if it is not
        //If the ID is already there, Greedy picked the same item twice
        if (chosenIDs.count(pickedID) > 0) {
            //I stop this test because the answer is invalid
            return false;
        }

        //I remember the ID so I can detect it if it appears again
        chosenIDs.insert(pickedID);

        //I have not found this ID in the original items yet.
        bool found = false;

        //I search the original input for the chosen ID
        //I need the original weight and value to check Greedy's answer
        for (const Item& item : items) {
            if (item.ID == pickedID) {
                totalValue += item.value;
                totalWeight += item.weight;
                found = true;
                break;
            }
        }
        //!found means found is false
        //if not, the answer is invalid
        if (!found) {
            return false;
        }
    }

    //All of these conditions must be true
    //1. The actual weight must fit in the bag
    //2. My calculated weight must equal Greedy's weight
    //3. My calculated value must equal Greedy's value
    return totalWeight <= capacity && totalWeight == answer.totalWeight && totalValue == answer.totalValue;
}   

//This is a seperate way to find the BEST possible value for a SMALL input
//it is not Greedy. It tries both choices for each item: skip it or take it
//I only use brute force for tests with 10 items
long long bruteForce(const std::vector<Item>& items, int capacity, int index) {

        //Base case: I have checked every item, so there is nothing left to add
        //this also stops the function from running forever
        if (index == static_cast<int>(items.size())) {
            return 0;
        }

        //I leave the current item out
        //I still have the same capacity, and I move to the next item
        long long skipValue = bruteForce(items, capacity, index + 1);

        //I take the current item, but ONLY if it can fit
        //I start at 0 because taking the item may be impossible
        long long takeValue = 0;

        if (items[index].weight <= capacity) {
            //I add this item's value, reduce the free capacity by its weight
            //and find the best value from the items that come after it
            takeValue = items[index].value + bruteForce(items, capacity - items[index].weight, index + 1);
        }
        //I choose the larger total from skipping or taking this item
        //and find the best value from the items that come after it
        return std::max(skipValue, takeValue);
    }

int main() {
    //I use 42 as the seed so I can generate
    //the same random inputs every time I run the test
    std::mt19937 randomNumber(42);

    //I will add the quality ratios together and divide by the number of valid ratios
    double sumOfRatios = 0.0;
    //1.0 means 100%. I will replace this if I see a worse ratio
    double lowestRatio = 1.0;
    //I count how many tests Greedy reaches the exact best Value
    int bestCount = 0;
    //I count how many ratios I actually calculate, so I do not divide by 0
    int ratioCount = 0;


    //I repeat the whole test 100 times
    //Each round creates new items and a new capacity
    for (int testNum = 1; testNum <= 100; testNum++) {
        std::vector<Item> items;

        //I create 10 items for each test
        //Item IDs go from 1 to 10, so they are all different
        for (int i = 1; i <= 10; i++) {
            //% 20 gives a remainder from 0 to 19; + 1 gives a weight from 1 to 20
            int weight = (randomNumber() % 20) + 1;

            //% 100 gives a remainder from 0 to 99; + 1 gives a value from 1 to 100
            long long value = (randomNumber() % 100) + 1;

            //I save the new item as {ID, weight, value}
            items.push_back({i, weight, value});
        }

        //I give this test a capacity from 1 to 50
        int capacity = (randomNumber() % 50) + 1;

        //I run Greedy using the generated input
        GreedyResult result = greedyKnapsack(items, capacity);

        //I check Greedy's chosen IDs and totals before comparing quality
        //!checkAnswer() means the test function returned false
        if (!checkAnswer(items, capacity, result)) {
            std::cout << "Test " << testNum << ": Fail" << std::endl;
            return 1;
        }

        //I find the true best value using brute force on this small input
        //The last argument is 0 because I start with the first item (index 0)
        long long bestValue = bruteForce(items, capacity, 0);

        //A valid Greedy answer cannot be worth MORE than the best value
        //if it is, the Greedy result or my check has a problem
        if (result.totalValue > bestValue) {
            std::cout << "Test " << testNum << ": Greedy value exceeds optimal value" << std::endl;
            return 1;
        }
        
        //I only calculate the ratio if bestValue is > 0
        //Else, I would be dividing by 0
        if (bestValue > 0) {
            //I convert the Greedy value to double so division keeps decimals
            //Example: 50 / 200 as integers is 0, but 50.0 / 200 is 0.25
            double ratio = static_cast<double>(result.totalValue) / bestValue;

            //I save this ratio for the average at the end
            sumOfRatios += ratio;

            //I count how many tests have a valid quality ratio
            ratioCount++;

            //I update the smallest ratio I have seen so far
            if (ratio < lowestRatio) {
                lowestRatio = ratio;
            }

            //if the values are equal, Greedy has found the best VALUE
            //it does not have to choose the exact same items
            if (result.totalValue == bestValue) {
                bestCount++;
            }

            //I multiply by 100 to show a percentage instead of a decimal
            //I only print this line once for each test
            std::cout << "Test " << testNum << " -Ratio: " << ratio * 100 << "%" << std::endl;
        }
    }
    
    // I calculate the average quality ratio from all valid tests.
    // I only divide if there is at least one valid ratio.
    if (ratioCount > 0) {
        //Example: a sum of 2.7 across 3 tests gives 0.9, or 90%
        double averageRatio = sumOfRatios / ratioCount;

        // I print the average and worst quality ratios.
        // I multiply by 100 to show them as percentages.
        std::cout << "Average Ratio: " << averageRatio * 100 << "%" << std::endl;

        std::cout << "Lowest Ratio: " << lowestRatio * 100 << "%" << std::endl;
    }

    // I print how many times Greedy found the optimal value.
    std::cout << "Optimal Solutions: " << bestCount << " / 100" << std::endl;

    //I only reach this line if none of the validity checks failed
    std::cout << "All 100 random tests: Pass" << std::endl;
    return 0;


}