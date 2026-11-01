#include <iostream>
#include <chrono>
#include <vector>
#include "GreedyKnapsack.hpp"
#include "DP\DPKnapsack.hpp"
#include "FPTAS\FPTAS.hpp"
#include <fstream>

int main () {
    //I create a CSV file (Excel) so my team can see the raw timing result
    std::ofstream csv("fullBenchmark.csv");

    //I write column names first, so it is easier to read the data later
    //n = number of items; run = batch number (each batch run Greedy 100 times); time_ms = average time per call
    //total_value = value returned by the Greedy algorithm
    csv << "n,run, dp_time, greedy_time, fptas_time, dp_value, greedy_value, fptas_value, epsilon" << std::endl;

    //I want to see how the runtime changes as the number of items increases
    //I use the same input sizes to compare Greedy, DP, and FPTAS
    std::vector<int> sizes = {100, 500, 1000, 5000, 10000};

    //I repeat the full benchmark for every size in the list above
    //In each round, n is the number of items I will create
    for (int n : sizes) {
        //I create the input before starting the timer
        //This means the time spent generating items is NOT part of the runtime result
        std::vector<Item> items;
        std::vector<int> itemValues;
        std::vector<int> itemWeights;
        std::vector<FPTASItem> fptasItems;

        //I create exactly n items. Each item has an ID, weight, and value
        //the modulo makes the weights and values follow a repeating pattern
        //This is NOT random input, so I should not claim it covers every type of data
        for (int i = 1; i <= n; i++) {
            //I use % to get the remainder after dividing i by 50
            //The remainder can be from 0 to 49
            //I add 1 so the weight is always between 1 and 50
            //This also prevents any item from having a weight of 0
            //After every 50 items, the weights repeat in the same pattern
            int weight = (i % 50) + 1;

            //I use % 100 to get a remainder between 0 and 99
            //I add 10 so the item values are between 10 and 109
            //This makes sure every item has a positive value
            //After every 100 items, the values repeat in the same pattern 
            long long value = (i % 100) + 10;

            //I add the new item in the order {ID, weight, value}
            items.push_back({i, weight, value});
            itemValues.push_back(value);
            itemWeights.push_back(weight);

            fptasItems.push_back({ weight, static_cast<int>(value) });
        } 
         


        //I set the bag capacity to half the number of items
        //Capacity therefore changes along with n in this experiment
        //This is one specific test setup, not a rule of Knapsack
        int capacity = n / 2;

        //I record the time just before running Greedy
        //auto start = std::chrono::steady_clock::now();

        //I use result to keep the answer from the latest Greedy call
        //It also lets me save the returned total value alongside the timing
        GreedyResult greedyResult;
        DPResult dpResult;
        std::vector<int> fptasResult;
        double epsilon = 0.2;

        //I run Greedy 10 times before measuring anything
        //These warm-up calls can reduce some first-run effects
        //I do not include their runtime in the CSV results
        for (int i =0; i < 10; i++) {
            greedyResult = greedyKnapsack(items, capacity);
        }

        //For each input size, I collect 10 seperate timing batches
        //Each batch calls Greedy 100 times on the SAME input data
        //This gives me 10 timing values per size so I can examine their spread
        for (int run  = 1; run <= 10; run++) {

            //I start the clock immediately before the measured calls
            auto start = std::chrono::steady_clock::now();

            //I call Greedy 100 times to make the measured period longer
            //I keep the result so the returned value is available after timing
            //This includes all work done INSIDE greedyKnapsack, such as sorting
            for (int i =0; i < 100; i++) {
                greedyResult = greedyKnapsack(items, capacity);

            }

            //I stop the timer immediately as soon as the 100 cals have finished
            //I do not print or write CSV data while the clock is running
            auto end = std::chrono::steady_clock::now();

            //I calculate the total elapsed time for these 100 calls in miliseconds
            double totalTimeMilisconds = std::chrono::duration<double, std::milli>(end - start).count();
            
            //I divide by 100 to estimate the average time for on Greedy call
            //This is an average WITHIN one batch, not the median of all 10 batches
            double averageTimeMilisecondsGreedy = totalTimeMilisconds / 100.0;

            //Repeat with DP
            start = std::chrono::steady_clock::now();
               for (int i =0; i < 100; i++) {
                dpResult = dpKnapsack(itemWeights, itemValues, capacity);
            }
            end = std::chrono::steady_clock::now();
            totalTimeMilisconds = std::chrono::duration<double, std::milli>(end - start).count();
            
            double averageTimeMilisecondsDP= totalTimeMilisconds / 100.0;

            //repeat with FPTAS
            start = std::chrono::steady_clock::now();
            for (int i = 0; i < 100; i++) {
                fptasResult = knapsackFPTAS(fptasItems, capacity, epsilon);
            }
            end = std::chrono::steady_clock::now();
            double totalTimeMilisecondsFPTAS = std::chrono::duration<double, std::milli>(end - start).count();

            double averageTimeMilisecondsFPTAS = totalTimeMilisecondsFPTAS / 100.0;

            int fptasTotalValue = 0;

            for (int index : fptasResult) {
                fptasTotalValue += fptasItems[index].value;
            }

            // I save one CSV row for this batch
            //Later, my team can calculate the median and variation for each size
            //and create graphs without manually copying numbers from the terminal
            csv << n << "," << run << "," << 
            averageTimeMilisecondsDP << "," << averageTimeMilisecondsGreedy << "," << averageTimeMilisecondsFPTAS << ","
            << dpResult.totalValue << "," << greedyResult.totalValue << "," << fptasTotalValue << "," << epsilon << "\n";
        }
        
        //I print only after all 10 batches for this size are finished
        //This helps me see progress without adding printing time to the measurements
        std::cout << "Finished benchmarking n = " << n << '\n';


        
    }
    return 0;

}
