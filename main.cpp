#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <chrono> // To measure exact speed in milliseconds
#include "Agent.hpp"

int main() {
    // 6.7 Million Agents (1:10 Scale of the UK Population)
    const int TOTAL_CITIZENS = 6700000;

    std::cout << "Allocating memory for " << TOTAL_CITIZENS << " UK citizens...\n";
    auto start_time = std::chrono::high_resolution_clock::now();

    std::vector<Agent> population;
    population.reserve(TOTAL_CITIZENS); // Crucial: reserves RAM upfront

    // Fast Pseudo-Random Generator
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    // Replace the old income line with a realistic income curve:
    std::lognormal_distribution<double> urban_income_dist(10.4, 0.4); // Median ~£33k, with variation
    std::lognormal_distribution<double> rural_income_dist(10.1, 0.35); // Median ~£24k, with variation

    // 1. Generate Population
    for (int i = 0; i < TOTAL_CITIZENS; ++i) {
        double roll = dist(rng);
        bool is_rural = (roll < 0.15);

        double income = is_rural ? rural_income_dist(rng) : urban_income_dist(rng);

        population.emplace_back(i, income, is_rural);
    }

    // 2. Apply Policy Shock (10% Tax)
    double tax_rate = 0.10;
    int protesting_count = 0;
    int rural_protest_count = 0;

    for (Agent& citizen : population) {
        citizen.applyTax(tax_rate);
        
        if (citizen.is_protesting) {
            protesting_count++;
            if (citizen.is_rural) {
                rural_protest_count++;
            }
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    // 3. Print Macro Results
    std::cout << "\n=== SIMULATION COMPLETE ===\n";
    std::cout << "Time Elapsed: " << std::fixed << std::setprecision(2) << elapsed.count() << " seconds\n";
    std::cout << "Total Protesters: " << protesting_count << " / " << TOTAL_CITIZENS 
              << " (" << (static_cast<double>(protesting_count) / TOTAL_CITIZENS) * 100.0 << "%)\n";
    std::cout << "Rural Protesters: " << rural_protest_count << "\n";

    return 0;
}