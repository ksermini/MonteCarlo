#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include "OptionType.h"

class Config {
public:
    /**
     * @brief Load configuration from a JSON file
     * 
     * @param filename Path to the JSON configuration file
     * @return Config Loaded configuration object
     */
    static Config load(const std::string& filename);

    // Simulation parameters
    unsigned int num_simulations;
    unsigned int num_threads;

    // Option parameters
    OptionType option_type;
    double S;      // Initial stock price
    double K;      // Strike price
    double r;      // Risk-free rate
    double sigma;  // Volatility
    double T;      // Time to maturity

    // Output parameters
    int precision;
    bool show_timing;

private:
    Config() = default;
    static OptionType parse_option_type(const std::string& type_str);
}; 