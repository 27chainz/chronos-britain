#pragma once

class Agent {
    public:
    int id;
    int age;
    bool is_rural;
    double income;
    double approval;

    Agent(int agent_id, int agent_age, bool agent_is_rural, double starting_income){
        id = agent_id;
        income = starting_income;
        approval =  0.45;
        age = agent_age;
        is_rural = agent_is_rural;

    }
    void applyTax(double tax_rate) {
        double tax_paid = income * tax_rate;
        income = income - tax_paid;
        approval = approval - (tax_paid / 1000.0) * 0.05;
    }
};