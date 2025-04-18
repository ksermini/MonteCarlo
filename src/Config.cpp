#include "Config.h"
#include <fstream>
#include <thread>
#include <stdexcept>

Config Config::load(const std::string& filename) {
    Config config;
    
    // Read JSON file
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open configuration file: " + filename);
    }

    nlohmann::json json;
    file >> json;

    // Parse simulation parameters
    config.num_simulations = json["simulation"]["num_simulations"];
    if (json["simulation"]["num_threads"] == "auto") {
        config.num_threads = std::thread::hardware_concurrency();
    } else {
        config.num_threads = json["simulation"]["num_threads"];
    }

    // Parse option parameters
    config.option_type = parse_option_type(json["option"]["type"]);
    const auto& params = json["option"]["parameters"];
    config.S = params["S"];
    config.K = params["K"];
    config.r = params["r"];
    config.sigma = params["sigma"];
    config.T = params["T"];

    // Parse output parameters
    config.precision = json["output"]["precision"];
    config.show_timing = json["output"]["show_timing"];

    return config;
}

OptionType Config::parse_option_type(const std::string& type_str) {
    if (type_str == "Call") {
        return OptionType::Call;
    } else if (type_str == "Put") {
        return OptionType::Put;
    } else {
        throw std::runtime_error("Invalid option type: " + type_str);
    }
} 