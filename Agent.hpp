#pragma once
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>

enum class Ethnicity {
    White,
    Asian,
    Black,
    MixedOther
};

enum class Region {
    London,
    South,
    Midlands,
    North,
    Scotland,
    Wales,
    NI
};

class Agent {
public:
    int id;
    int age;
    Ethnicity ethnicity;
    Region region;
    bool is_rural;
    bool is_homeowner;

    double annual_gross_income;
    double base_income;

    double monthly_savings;
    double base_savings;

    double approval;
    double base_approval;

    bool is_protesting;
    
    // Small-world network long-range peer shortcut ID (-1 if none)
    int long_range_peer_id;

    Agent(int agent_id, int agent_age, double starting_income, bool agent_is_rural, Ethnicity agent_ethnicity, Region agent_region, bool agent_is_homeowner, int shortcut_id = -1) {
        id = agent_id;
        age = agent_age;
        annual_gross_income = starting_income;
        base_income = starting_income;
        is_rural = agent_is_rural;
        is_homeowner = agent_is_homeowner;
        ethnicity = agent_ethnicity;
        region = agent_region;
        is_protesting = false;
        long_range_peer_id = shortcut_id;

        // Baseline liquid savings based on age & homeownership (in £)
        double initial_savings = (age * 150.0) + (is_homeowner ? 5000.0 : 500.0);
        monthly_savings = initial_savings;
        base_savings = initial_savings;

        switch(ethnicity) {
            case Ethnicity::White: approval = 0.50; break;
            case Ethnicity::Asian: approval = 0.52; break;
            case Ethnicity::Black: approval = 0.42; break;
            case Ethnicity::MixedOther: approval = 0.45; break;
        }

        // Demographic & regional stability modifiers
        if (age >= 65) approval += 0.05;
        if (age <= 30) approval -= 0.05;
        if (is_homeowner) approval += 0.05;
        if (region == Region::London) approval -= 0.05;
        if (region == Region::North || region == Region::Scotland) approval -= 0.02;

        base_approval = approval;
    }

    void resetState() {
        annual_gross_income = base_income;
        monthly_savings = base_savings;
        approval = base_approval;
        is_protesting = false;
    }

    // UK Progressive Tax Band Calculation
    static double calculateNetMonthlyIncome(double gross_annual, double tax_modifier_pct) {
        double personal_allowance = 12570.0;
        double basic_rate_cap = 50270.0;

        double taxable = std::max(0.0, gross_annual - personal_allowance);
        double tax = 0.0;

        if (taxable > 0) {
            double basic_taxable = std::min(taxable, basic_rate_cap - personal_allowance);
            tax += basic_taxable * (0.20 + tax_modifier_pct);

            double higher_taxable = std::max(0.0, taxable - basic_taxable);
            tax += higher_taxable * (0.40 + tax_modifier_pct);
        }

        double net_annual = gross_annual - tax;
        return net_annual / 12.0;
    }

    // Process a single month step
    void processMonthStep(double inflation_rate, double tax_modifier_pct, double monthly_welfare_boost, double energy_subsidy) {
        double net_monthly = calculateNetMonthlyIncome(annual_gross_income, tax_modifier_pct) + monthly_welfare_boost;

        // Inelastic essential living costs (food, energy, rent/mortgage)
        double base_essential_cost = 1000.0; // £1,000/mo baseline
        
        // Renters hit harder by inflation
        double housing_multiplier = is_homeowner ? 0.9 : 1.3;
        double regional_multiplier = (region == Region::London) ? 1.4 : 1.0;

        double adjusted_essential_cost = (base_essential_cost * housing_multiplier * regional_multiplier * (1.0 + inflation_rate)) - energy_subsidy;
        adjusted_essential_cost = std::max(200.0, adjusted_essential_cost);

        double net_cashflow = net_monthly - adjusted_essential_cost;
        monthly_savings += net_cashflow;

        // Severe economic distress if savings depleted or negative cashflow
        if (monthly_savings < 0) {
            approval -= 0.08; // Rapid drop when in deficit
        } else if (net_cashflow < 0) {
            approval -= 0.03; // Gradual drop when burning savings
        } else {
            approval += 0.01; // Slight recovery when surplus
        }

        approval = std::clamp(approval, 0.0, 1.0);

        // Protest threshold (Younger citizens protest at higher approval rates)
        double protest_threshold = (age < 30) ? 0.38 : 0.30;
        if (approval < protest_threshold) {
            is_protesting = true;
        } else {
            is_protesting = false;
        }
    }

    void applyPeerPressure() {
        approval -= 0.04;
        if (approval < 0.38) {
            is_protesting = true;
        }
    }
};