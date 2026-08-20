#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <chrono> // To measure exact speed in milliseconds
#include <unistd.h>
#include <cstdio>
#include "Agent.hpp"
#include "DataEngine.hpp"

int main() {
    // 6.7 Million Agents (1:10 Scale of the UK Population)
    const int TOTAL_CITIZENS = 6700000;

    DataEngine dataEngine;
    dataEngine.loadSchema("data/uk_agent_schema.csv");

    std::cout << "Allocating memory for " << TOTAL_CITIZENS << " UK citizens...\n";
    auto start_time = std::chrono::high_resolution_clock::now();

    std::vector<Agent> population;
    population.reserve(TOTAL_CITIZENS); // Crucial: reserves RAM upfront

    // Fast Pseudo-Random Generator
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    // Realistic income distributions by ethnicity (approximate lognormal)
    std::lognormal_distribution<double> white_income_dist(10.37, 0.4); // Median ~£32k
    std::lognormal_distribution<double> asian_income_dist(10.43, 0.4); // Median ~£34k
    std::lognormal_distribution<double> black_income_dist(10.12, 0.4); // Median ~£25k
    std::lognormal_distribution<double> mixed_income_dist(10.24, 0.4); // Median ~£28k

    // Age distribution (approx normal around 42 years, clamp 18-90)
    std::normal_distribution<double> age_dist(42.0, 15.0);

    // 1. Generate Population
    for (int i = 0; i < TOTAL_CITIZENS; ++i) {
        double roll_eth = dist(rng);
        Ethnicity eth;
        double income;

        if (roll_eth < 0.817) { // 81.7% White
            eth = Ethnicity::White;
            income = white_income_dist(rng);
        } else if (roll_eth < 0.910) { // 9.3% Asian
            eth = Ethnicity::Asian;
            income = asian_income_dist(rng);
        } else if (roll_eth < 0.950) { // 4.0% Black
            eth = Ethnicity::Black;
            income = black_income_dist(rng);
        } else { // 5.0% Mixed / Other
            eth = Ethnicity::MixedOther;
            income = mixed_income_dist(rng);
        }

        double roll_rural = dist(rng);
        bool is_rural = (roll_rural < 0.15);
        if (is_rural) {
            income *= 0.8; // Apply 20% rural income penalty
        }

        double generated_age = age_dist(rng);
        int age = std::max(18, std::min(90, static_cast<int>(generated_age)));

        // Homeownership depends strongly on age
        double roll_home = dist(rng);
        bool is_homeowner = false;
        if (age > 40) is_homeowner = (roll_home < 0.70); // 70% of over 40s
        else is_homeowner = (roll_home < 0.35); // 35% of under 40s

        // Region Generation
        double roll_region = dist(rng);
        Region region;
        if (roll_region < 0.13) {
            region = Region::London;
            income *= 1.30; // 30% London premium
        } else if (roll_region < 0.36) {
            region = Region::South;
            income *= 1.10;
        } else if (roll_region < 0.52) {
            region = Region::Midlands;
        } else if (roll_region < 0.76) {
            region = Region::North;
            income *= 0.90;
        } else if (roll_region < 0.84) {
            region = Region::Scotland;
            income *= 0.95;
        } else if (roll_region < 0.89) {
            region = Region::Wales;
            income *= 0.90;
        } else {
            region = Region::NI;
            income *= 0.90;
        }

        population.emplace_back(i, age, income, is_rural, eth, region, is_homeowner);
    }

    // 2. Interactive Policy Input Shell
    double tax_rate = 0.05;
    double inflation_rate = 0.08;

    std::cout << "\n============================================\n";
    std::cout << "  UK SOCIOECONOMIC SIMULATION - POLICY ENGINE\n";
    std::cout << "============================================\n";
    std::cout << "Loaded " << TOTAL_CITIZENS << " agents with ONS demographic models.\n\n";

    if (isatty(fileno(stdin))) {
        std::cout << "Enter proposed Income Tax Rate % (e.g. 5 for 5%): ";
        if (!(std::cin >> tax_rate)) tax_rate = 5.0;
        tax_rate /= 100.0;

        std::cout << "Enter proposed Inflation Rate % (e.g. 8 for 8%): ";
        if (!(std::cin >> inflation_rate)) inflation_rate = 8.0;
        inflation_rate /= 100.0;
    } else {
        std::cout << "Non-interactive mode detected. Using default Tax: 5%, Inflation: 8%\n";
    }

    std::cout << "\nRunning policy simulation with Tax Rate = " << (tax_rate * 100.0) 
              << "% and Inflation = " << (inflation_rate * 100.0) << "%...\n";
    
    // Apply economic shock to all citizens
    for (Agent& citizen : population) {
        citizen.applyEconomicShock(inflation_rate, tax_rate);
    }

    // 3. Apply Peer Pressure (Ring Lattice Network)
    int protesting_count = 0;
    int rural_protest_count = 0;
    int london_protest_count = 0;

    for (int i = 0; i < TOTAL_CITIZENS; ++i) {
        int peers_protesting = 0;
        
        // Check nearest neighbors (i-2, i-1, i+1, i+2) with wrap-around
        for (int offset : {-2, -1, 1, 2}) {
            int peer_idx = (i + offset + TOTAL_CITIZENS) % TOTAL_CITIZENS;
            if (population[peer_idx].is_protesting) {
                peers_protesting++;
            }
        }

        // If >= 50% (2 out of 4) friends are protesting, peer pressure applies
        if (peers_protesting >= 2) {
            population[i].applyPeerPressure();
        }

        // Tally final counts
        if (population[i].is_protesting) {
            protesting_count++;
            if (population[i].is_rural) {
                rural_protest_count++;
            }
            if (population[i].region == Region::London) {
                london_protest_count++;
            }
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    // 4. Print Macro & Demographic Results
    std::cout << "\n=== SIMULATION RESULTS ===\n";
    std::cout << "Time Elapsed: " << std::fixed << std::setprecision(2) << elapsed.count() << " seconds\n";
    std::cout << "Total Protesters: " << protesting_count << " / " << TOTAL_CITIZENS 
              << " (" << (static_cast<double>(protesting_count) / TOTAL_CITIZENS) * 100.0 << "%)\n";
    std::cout << "Rural Protesters: " << rural_protest_count << "\n";
    std::cout << "London Protesters: " << london_protest_count << "\n";

    return 0;
}