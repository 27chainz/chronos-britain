#pragma once

enum class Ethnicity { White, Asian, Black, MixedOther };
enum class Region { London, South, Midlands, North, Scotland, Wales, NI };

class Agent {
    public:
    int id;
    int age;
    bool is_rural;
    bool is_homeowner;
    Ethnicity ethnicity;
    Region region;
    double income;
    double base_income;
    double approval;
    double base_approval;

    bool is_protesting;

    Agent(int agent_id, int agent_age, double starting_income, bool agent_is_rural, Ethnicity agent_ethnicity, Region agent_region, bool agent_is_homeowner){
        id = agent_id;
        age = agent_age;
        income = starting_income;
        base_income = starting_income;
        is_rural = agent_is_rural;
        is_homeowner = agent_is_homeowner;
        ethnicity = agent_ethnicity;
        region = agent_region;
        is_protesting = false;

        switch(ethnicity) {
            case Ethnicity::White: approval = 0.45; break;
            case Ethnicity::Asian: approval = 0.50; break;
            case Ethnicity::Black: approval = 0.35; break;
            case Ethnicity::MixedOther: approval = 0.40; break;
        }

        // Generational base stability adjustments
        if (age >= 65) approval += 0.05;
        if (age <= 30) approval -= 0.05;
        
        // Homeownership stability
        if (is_homeowner) approval += 0.05;
        
        // Regional baseline adjustments
        if (region == Region::London) approval -= 0.05;
        if (region == Region::North || region == Region::Scotland) approval -= 0.02;

        base_approval = approval;
    }
    
    void resetState() {
        income = base_income;
        approval = base_approval;
        is_protesting = false;
    }
    
    void applyEconomicShock(double inflation_rate, double tax_rate) {
        // Pensioners are partially insulated from wage taxes but hit by inflation on fixed incomes
        double effective_tax_rate = (age >= 65) ? (tax_rate * 0.5) : tax_rate;
        double tax_paid = income * effective_tax_rate;
        
        // Inflation eats away at purchasing power. Renters feel this MUCH harder due to housing costs.
        double cost_of_living_increase = income * inflation_rate * (is_homeowner ? 0.8 : 1.5);
        
        income = income - tax_paid;
        
        // Sensitivity to economic shock varies by age and wealth
        double age_sensitivity = (age <= 30) ? 1.2 : ((age >= 65) ? 0.8 : 1.0);
        double total_financial_hit = tax_paid + cost_of_living_increase;
        
        approval = approval - (total_financial_hit / 1000.0) * 0.03 * age_sensitivity;
        
        // Youth and renters are quicker to organize/protest
        double protest_threshold = (age <= 30 || !is_homeowner) ? 0.35 : 0.30;
        is_protesting = (approval < protest_threshold);
    }

    void applyPeerPressure() {
        approval -= 0.05;
        double protest_threshold = (age <= 30) ? 0.35 : 0.30;
        is_protesting = (approval < protest_threshold);
    }
};