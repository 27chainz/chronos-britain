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

    // 2. Interactive Policy Input Shell (REPL)
    std::cout << "\n============================================\n";
    std::cout << "  UK SOCIOECONOMIC SIMULATION - POLICY ENGINE\n";
    std::cout << "============================================\n";
    std::cout << "Loaded " << TOTAL_CITIZENS << " agents with ONS demographic models.\n";
    std::cout << "Enter policy parameters to observe population reactions.\n";

    while (true) {
        double tax_input = 0.0;
        double inflation_input = 0.0;

        std::cout << "\n--------------------------------------------\n";
        if (isatty(fileno(stdin))) {
            std::cout << "Enter Income Tax Rate % (e.g., 5 for 5%, -1 to exit): ";
            if (!(std::cin >> tax_input) || tax_input < 0) {
                std::cout << "Exiting simulation shell.\n";
                break;
            }

            std::cout << "Enter Inflation Rate % (e.g., 8 for 8%): ";
            if (!(std::cin >> inflation_input)) break;
        } else {
            std::cout << "Non-interactive mode detected. Running test shock (Tax: 5%, Inflation: 8%)\n";
            tax_input = 5.0;
            inflation_input = 8.0;
        }

        double tax_rate = tax_input / 100.0;
        double inflation_rate = inflation_input / 100.0;

        auto step_start = std::chrono::high_resolution_clock::now();

        // Reset agent state & apply new economic shock
        for (Agent& citizen : population) {
            citizen.resetState();
            citizen.applyEconomicShock(inflation_rate, tax_rate);
        }

        // Apply Peer Pressure (Ring Lattice Network)
        int protesting_count = 0;
        int rural_protest_count = 0;
        int london_protest_count = 0;

        for (int i = 0; i < TOTAL_CITIZENS; ++i) {
            int peers_protesting = 0;
            
            for (int offset : {-2, -1, 1, 2}) {
                int peer_idx = (i + offset + TOTAL_CITIZENS) % TOTAL_CITIZENS;
                if (population[peer_idx].is_protesting) {
                    peers_protesting++;
                }
            }

            if (peers_protesting >= 2) {
                population[i].applyPeerPressure();
            }

            if (population[i].is_protesting) {
                protesting_count++;
                if (population[i].is_rural) rural_protest_count++;
                if (population[i].region == Region::London) london_protest_count++;
            }
        }

        auto step_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = step_end - step_start;

        std::cout << "\n=== POLICY REACTION RESULTS ===\n";
        std::cout << "Policy Applied   : Tax = " << tax_input << "%, Inflation = " << inflation_input << "%\n";
        std::cout << "Reaction Time    : " << std::fixed << std::setprecision(2) << elapsed.count() << " seconds\n";
        std::cout << "Total Protesters : " << protesting_count << " / " << TOTAL_CITIZENS 
                  << " (" << (static_cast<double>(protesting_count) / TOTAL_CITIZENS) * 100.0 << "%)\n";
        std::cout << "London Protesters: " << london_protest_count << "\n";
        std::cout << "Rural Protesters : " << rural_protest_count << "\n";

        if (!isatty(fileno(stdin))) break; // Exit loop if piped/non-interactive
    }

    return 0;
}