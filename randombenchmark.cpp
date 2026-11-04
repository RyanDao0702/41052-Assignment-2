#include <iostream>
#include <chrono>
#include <vector>
#include "Greedy\GreedyKnapsack.hpp"
#include "DP\DPKnapsack.hpp"
#include <fstream>
#include <random>
#include "FPTAS\FPTAS.hpp"

int main () {
    //I create a CSV file (Excel) so my team can see the raw timing result
    std::ofstream csv("randomBenchmarkSmall.csv");

    //I write column names first, so it is easier to read the data later
    //n = number of items; run = batch number, time_ms = average time per call
    //value = value returned by the corresponding algorithm
    csv << "n,c,run,dp_time,greedy_time,fptas_time,dp_value,greedy_value, fptas_value,epsilon" << std::endl;
    //Without FPTAS
    // csv << "n,c,run,dp_time,greedy_time,dp_value,greedy_value" << std::endl;

    //Use a fixed seed for randomly generating numbers to make each run of the benchmark the same (for comparing benchmarks)
    std::mt19937 gen(42);

    //Test with every combination of 5 sizes and 3 capacities
    std::vector<int> sizes = {100, 500, 1000};
    std::vector<int> capacities = {100, 1000, 10000};

    std::vector<std::pair<int, int>> tests;
    for (int size : sizes) {
        for (int capacity : capacities) {
            tests.push_back({size, capacity});
        }
    }

    //Repeat the benchmark for each combination of n and c
    //In each round, n is the number of items and c is the capacity
    for (std::pair<int, int> test : tests) {

        int n = test.first;
        int c = test.second;


        //For the random test, each run is done with different items and weights
        for (int run  = 1; run <= 10; run++) {

            //I create the input before starting the timer
            //This means the time spent generating items is NOT part of the runtime result
            std::vector<Item> items;
            std::vector<int> itemValues;
            std::vector<int> itemWeights;
            std::vector<FPTASItem> fptasItems;

    
            //Weights and be randomised between 1 and the capacity
            //No items will be heavier than the capacity
            std::uniform_int_distribution<> weightDistrib(1, c);

            //Values are distributed between 1 and 1000, regardless of capacity
            std::uniform_int_distribution<> valueDistrib(1, 1000);


            //Create exactly n. Each item has an ID, weight, and value
            for (int i = 1; i <= n; i++) { 
                
                //each item is a random weight and value
                int weight = weightDistrib(gen);
                long long value = valueDistrib(gen);

            //Add the new item
                items.push_back({i, weight, value});
                itemValues.push_back(value);
                itemWeights.push_back(weight);
                fptasItems.push_back({ weight, static_cast<int>(value) });
            } 
            

            int capacity = c;

            //use result to keep the max value from the latest call
            GreedyResult greedyResult;
            DPResult dpResult;
            std::vector<int> fptasResult;
            double epsilon = 0.2;

            //start the clock immediately before the measured calls
            auto start = std::chrono::steady_clock::now();

            //I keep the result so the returned value is available after timing
            //This includes all work done INSIDE greedyKnapsack, such as sorting
            for (int i =0; i < 10; i++) {
                greedyResult = greedyKnapsack(items, capacity);

            }

            //I stop the timer immediately as soon as the 100 cals have finished
            //I do not print or write CSV data while the clock is running
            auto end = std::chrono::steady_clock::now();

            //I calculate the total elapsed time for these 100 calls in miliseconds
            double totalTimeMilisconds = std::chrono::duration<double, std::milli>(end - start).count();
            
            //I divide by 10 to estimate the average time for on Greedy call
            //This is an average WITHIN one batch, not the median of all 10 batches
            double averageTimeMilisecondsGreedy = totalTimeMilisconds / 10.0;

            //Repeat with DP
            start = std::chrono::steady_clock::now();
                for (int i =0; i < 10; i++) {
                dpResult = dpKnapsack(itemWeights, itemValues, capacity);
            }
            end = std::chrono::steady_clock::now();
            totalTimeMilisconds = std::chrono::duration<double, std::milli>(end - start).count();
            
            double averageTimeMilisecondsDP= totalTimeMilisconds / 10.0;

            //repeat with FPTAS (only average 3 times, as FPTAS is slower)
            start = std::chrono::steady_clock::now();
            for (int i = 0; i < 3; i++) {
                fptasResult = knapsackFPTAS(fptasItems, capacity, epsilon);
            }
            end = std::chrono::steady_clock::now();
            double totalTimeMilisecondsFPTAS = std::chrono::duration<double, std::milli>(end - start).count();

            double averageTimeMilisecondsFPTAS = totalTimeMilisecondsFPTAS / 3.0;

            int fptasTotalValue = 0;

            for (int index : fptasResult) {
                fptasTotalValue += fptasItems[index].value;
            }

            // I save one CSV row for this batch
            //Later, my team can calculate the median and variation for each size
            //and create graphs without manually copying numbers from the terminal
            csv << n << "," << c << "," << run << "," << 
            averageTimeMilisecondsDP << "," << averageTimeMilisecondsGreedy << "," << averageTimeMilisecondsFPTAS << ","
            << dpResult.totalValue << "," << greedyResult.totalValue << "," << fptasTotalValue << "," << epsilon << "\n";

            //Without FPTAS
            //  csv << n << "," << c << "," << run << "," << 
            // averageTimeMilisecondsDP << "," << averageTimeMilisecondsGreedy << ","
            // << dpResult.totalValue << "," << greedyResult.totalValue << "\n";

            csv.flush();

    

        }

                
        //print only after all 10 batches for this size are finished
        //This helps me see progress without adding printing time to the measurements
        std::cout << "Finished benchmarking n = " << n << " c = " << c << '\n';

        
    }
    return 0;

}
