#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <chrono>
#include <unistd.h>
#include <cstdio>
#include "Agent.hpp"
#include "DataEngine.hpp"

int main() {
    const int TOTAL_CITIZENS = 6700000;

    DataEngine dataEngine;
    dataEngine.loadSchema("data/uk_agent_schema.csv");

    std::cout << "Allocating memory and generating Watts-Strogatz Small-World network for " << TOTAL_CITIZENS << " UK citizens...\n";
    auto start_time = std::chrono::high_resolution_clock::now();

    std::vector<Agent> population;
    population.reserve(TOTAL_CITIZENS);

    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    std::uniform_int_distribution<int> citizen_dist(0, TOTAL_CITIZENS - 1);

    std::lognormal_distribution<double> white_income_dist(10.37, 0.4);
    std::lognormal_distribution<double> asian_income_dist(10.43, 0.4);
    std::lognormal_distribution<double> black_income_dist(10.12, 0.4);
    std::lognormal_distribution<double> mixed_income_dist(10.24, 0.4);

    std::normal_distribution<double> age_dist(42.0, 15.0);

    // Generate Population with Small-World Long-Range Shortcuts (Watts-Strogatz topology)
    for (int i = 0; i < TOTAL_CITIZENS; ++i) {
        double roll_eth = dist(rng);
        Ethnicity eth;
        double income;

        if (roll_eth < 0.817) {
            eth = Ethnicity::White;
            income = white_income_dist(rng);
        } else if (roll_eth < 0.910) {
            eth = Ethnicity::Asian;
            income = asian_income_dist(rng);
        } else if (roll_eth < 0.950) {
            eth = Ethnicity::Black;
            income = black_income_dist(rng);
        } else {
            eth = Ethnicity::MixedOther;
            income = mixed_income_dist(rng);
        }

        bool is_rural = (dist(rng) < 0.15);
        if (is_rural) income *= 0.8;

        int age = std::max(18, std::min(90, static_cast<int>(age_dist(rng))));

        bool is_homeowner = (age > 40) ? (dist(rng) < dataEngine.homeownership_over_40) 
                                      : (dist(rng) < dataEngine.homeownership_under_40);

        double roll_region = dist(rng);
        Region region;
        if (roll_region < 0.13) {
            region = Region::London;
            income *= 1.30;
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

        // Watts-Strogatz rewire probability: 5% chance of long-range social shortcut
        int shortcut_id = -1;
        if (dist(rng) < 0.05) {
            shortcut_id = citizen_dist(rng);
        }

        population.emplace_back(i, age, income, is_rural, eth, region, is_homeowner, shortcut_id);
    }

    auto gen_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> gen_elapsed = gen_end - start_time;
    std::cout << "Population & Small-World Graph loaded in " << std::fixed << std::setprecision(2) << gen_elapsed.count() << " seconds.\n";

    // Interactive Policy Shell
    std::cout << "\n=========================================================\n";
    std::cout << "  UK SOCIOECONOMIC SIMULATION - ADVANCED POLICY ENGINE\n";
    std::cout << "=========================================================\n";

    while (true) {
        double tax_mod_input = 0.0;
        double inflation_input = 0.0;
        double welfare_boost_input = 0.0;
        double energy_subsidy_input = 0.0;
        int sim_months = 12;

        std::cout << "\n---------------------------------------------------------\n";
        if (isatty(fileno(stdin))) {
            std::cout << "Enter Tax Adjustment % (+/- e.g. 0 for baseline, 5 for +5%, -100 to exit): ";
            if (!(std::cin >> tax_mod_input) || tax_mod_input <= -100) {
                std::cout << "Exiting simulation engine.\n";
                break;
            }

            std::cout << "Enter Inflation Rate % (e.g., 8 for 8%): ";
            if (!(std::cin >> inflation_input)) break;

            std::cout << "Enter Monthly Universal Credit / Welfare Support (£/mo, e.g. 100): ";
            if (!(std::cin >> welfare_boost_input)) break;

            std::cout << "Enter Monthly Energy Bill Cap Subsidy (£/mo, e.g. 50): ";
            if (!(std::cin >> energy_subsidy_input)) break;

            std::cout << "Enter Simulation Timeline (Months, e.g. 12): ";
            if (!(std::cin >> sim_months)) sim_months = 12;
        } else {
            std::cout << "Non-interactive mode: Testing Policy (Tax +5%, Inflation 8%, Welfare £50/mo, Energy £25/mo, 6 Months)\n";
            tax_mod_input = 5.0;
            inflation_input = 8.0;
            welfare_boost_input = 50.0;
            energy_subsidy_input = 25.0;
            sim_months = 6;
        }

        double tax_mod_pct = tax_mod_input / 100.0;
        double inflation_rate = inflation_input / 100.0;

        // Reset Population State
        for (Agent& citizen : population) {
            citizen.resetState();
        }

        std::cout << "\nSimulating " << sim_months << "-month timeline under proposed policy...\n";
        std::cout << "Month | Total Protesters | % Pop  | London  | Rural\n";
        std::cout << "---------------------------------------------------\n";

        auto sim_start = std::chrono::high_resolution_clock::now();

        // Multi-Month Execution Loop
        for (int m = 1; m <= sim_months; ++m) {
            // Step 1: Process monthly personal cashflow & savings depletion
            for (Agent& citizen : population) {
                citizen.processMonthStep(inflation_rate, tax_mod_pct, welfare_boost_input, energy_subsidy_input);
            }

            // Step 2: Apply Peer Contagion on Small-World Graph
            int total_protesting = 0;
            int london_protesting = 0;
            int rural_protesting = 0;

            for (int i = 0; i < TOTAL_CITIZENS; ++i) {
                int peer_protests = 0;
                
                // Ring lattice 4 nearest neighbors
                for (int offset : {-2, -1, 1, 2}) {
                    int peer_idx = (i + offset + TOTAL_CITIZENS) % TOTAL_CITIZENS;
                    if (population[peer_idx].is_protesting) peer_protests++;
                }

                // Small-world long-range shortcut peer
                if (population[i].long_range_peer_id != -1 && population[population[i].long_range_peer_id].is_protesting) {
                    peer_protests++;
                }

                // Peer pressure triggers if >= 2 connections are protesting
                if (peer_protests >= 2) {
                    population[i].applyPeerPressure();
                }

                if (population[i].is_protesting) {
                    total_protesting++;
                    if (population[i].region == Region::London) london_protesting++;
                    if (population[i].is_rural) rural_protesting++;
                }
            }

            double pct = (static_cast<double>(total_protesting) / TOTAL_CITIZENS) * 100.0;
            std::cout << std::setw(5) << m << " | "
                      << std::setw(16) << total_protesting << " | "
                      << std::setw(5) << std::fixed << std::setprecision(1) << pct << "% | "
                      << std::setw(7) << london_protesting << " | "
                      << std::setw(6) << rural_protesting << "\n";
        }

        auto sim_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> sim_elapsed = sim_end - sim_start;

        std::cout << "\nTimeline Complete! Computation time: " << std::fixed << std::setprecision(2) << sim_elapsed.count() << "s\n";

        if (!isatty(fileno(stdin))) break;
    }

    return 0;
}