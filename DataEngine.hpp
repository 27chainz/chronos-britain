#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct DemographicRule {
    std::string category;
    double weight;
    double modifier;
};

class DataEngine {
public:
    std::vector<DemographicRule> ethnicities;
    std::vector<DemographicRule> regions;
    double homeownership_under_40 = 0.35;
    double homeownership_over_40 = 0.70;

    bool loadSchema(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Warning: Could not open " << filepath << ". Using default parameters.\n";
            return false;
        }

        std::string line;
        std::getline(file, line);

        while (std::getline(file, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string metric, category, weight_str, modifier_str;
            
            if (std::getline(ss, metric, ',') &&
                std::getline(ss, category, ',') &&
                std::getline(ss, weight_str, ',') &&
                std::getline(ss, modifier_str, ',')) {

                try {
                    double weight = std::stod(weight_str);
                    double modifier = std::stod(modifier_str);

                    if (metric == "ethnicity") {
                        ethnicities.push_back({category, weight, modifier});
                    } else if (metric == "region") {
                        regions.push_back({category, weight, modifier});
                    } else if (metric == "homeownership") {
                        if (category == "under_40") homeownership_under_40 = weight;
                        if (category == "over_40") homeownership_over_40 = weight;
                    }
                } catch (...) {
                    continue;
                }
            }
        }
        file.close();
        std::cout << "Loaded normalized schema from " << filepath << " successfully.\n";
        return true;
    }
};
