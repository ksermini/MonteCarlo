#include "montecarlo/ConfigLoader.h"
#include <fstream>
#include <thread>
#include <stdexcept>
#include <cmath>

namespace montecarlo {

Config Config::load(const std::string& filename, const CLIOptions& cli_options) {
    Config config;
    
    // Read JSON file
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw ConfigError("Failed to open configuration file: " + filename);
    }

    nlohmann::json json;
    try {
        file >> json;
    } catch (const nlohmann::json::parse_error& e) {
        throw ConfigError("Failed to parse configuration file: " + std::string(e.what()));
    }

    // Parse simulation parameters
    try {
        config.num_simulations = json["simulation"]["num_simulations"];
        if (json["simulation"]["num_threads"] == "auto") {
            config.num_threads = std::thread::hardware_concurrency();
        } else {
            config.num_threads = json["simulation"]["num_threads"];
        }
    } catch (const nlohmann::json::exception& e) {
        throw ConfigError("Invalid simulation parameters: " + std::string(e.what()));
    }

    // Parse option parameters
    try {
        config.option_type = parse_option_type(json["option"]["type"]);
        const auto& params = json["option"]["parameters"];
        config.S = params["S"];
        config.K = params["K"];
        config.r = params["r"];
        config.sigma = params["sigma"];
        config.T = params["T"];
    } catch (const nlohmann::json::exception& e) {
        throw ConfigError("Invalid option parameters: " + std::string(e.what()));
    }

    // Parse output parameters
    try {
        config.precision = json["output"]["precision"];
        config.show_timing = json["output"]["show_timing"];
    } catch (const nlohmann::json::exception& e) {
        throw ConfigError("Invalid output parameters: " + std::string(e.what()));
    }

    // Apply CLI overrides
    config.apply_cli_overrides(cli_options);

    // Validate configuration
    config.validate();

    return config;
}

void Config::validate() const {
    // Validate simulation parameters
    if (num_simulations == 0) {
        throw ConfigError("Number of simulations must be positive");
    }
    if (num_threads == 0) {
        throw ConfigError("Number of threads must be positive");
    }

    // Validate option parameters
    if (S <= 0) {
        throw ConfigError("Initial stock price must be positive");
    }
    if (K <= 0) {
        throw ConfigError("Strike price must be positive");
    }
    if (r < 0) {
        throw ConfigError("Risk-free rate cannot be negative");
    }
    if (sigma <= 0) {
        throw ConfigError("Volatility must be positive");
    }
    if (T <= 0) {
        throw ConfigError("Time to maturity must be positive");
    }

    // Validate output parameters
    if (precision < 0) {
        throw ConfigError("Precision must be non-negative");
    }
}

void Config::apply_cli_overrides(const CLIOptions& options) {
    if (options.num_simulations) num_simulations = *options.num_simulations;
    if (options.num_threads) num_threads = *options.num_threads;
    if (options.option_type) option_type = parse_option_type(*options.option_type);
    if (options.S) S = *options.S;
    if (options.K) K = *options.K;
    if (options.r) r = *options.r;
    if (options.sigma) sigma = *options.sigma;
    if (options.T) T = *options.T;
    if (options.precision) precision = *options.precision;
    if (options.show_timing) show_timing = *options.show_timing;
    dry_run = options.dry_run;
    validate_config = options.validate_config;
}

OptionType Config::parse_option_type(const std::string& type_str) {
    if (type_str == "Call") {
        return OptionType::Call;
    } else if (type_str == "Put") {
        return OptionType::Put;
    } else {
        throw ConfigError("Invalid option type: " + type_str);
    }
}

} // namespace montecarlo 