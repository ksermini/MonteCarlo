#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include "montecarlo/OptionType.h"
#include <optional>
#include <memory>

namespace montecarlo {

class ConfigError : public std::runtime_error {
public:
    explicit ConfigError(const std::string& message) : std::runtime_error(message) {}
};

class Config {
public:
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
    bool dry_run;
    bool validate_config;

    // CLI overrides
    struct CLIOptions {
        std::optional<unsigned int> num_simulations;
        std::optional<unsigned int> num_threads;
        std::optional<std::string> option_type;
        std::optional<double> S;
        std::optional<double> K;
        std::optional<double> r;
        std::optional<double> sigma;
        std::optional<double> T;
        std::optional<int> precision;
        std::optional<bool> show_timing;
        bool dry_run = false;
        bool validate_config = false;
    };

    static Config load(const std::string& filename, const CLIOptions& cli_options = {});
    void validate() const;

private:
    Config() = default;
    static OptionType parse_option_type(const std::string& type_str);
    void apply_cli_overrides(const CLIOptions& options);
};

} // namespace montecarlo 