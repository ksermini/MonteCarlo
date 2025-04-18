#include "Config.h"
#include <fstream>
#include <thread>
#include <stdexcept>

namespace montecarlo {

Config Config::load(const std::string& filename) {
    Config config;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open config file: " + filename);
    }

    nlohmann::json j;
    file >> j;

    // Load simulation parameters
    config.num_simulations = j["num_simulations"].get<unsigned int>();
    config.num_threads = j["num_threads"].get<unsigned int>();

    // Load option parameters
    config.option_type = parse_option_type(j["option_type"].get<std::string>());
    config.S = j["S"].get<double>();
    config.K = j["K"].get<double>();
    config.r = j["r"].get<double>();
    config.sigma = j["sigma"].get<double>();
    config.T = j["T"].get<double>();

    // Load output parameters
    config.precision = j["precision"].get<int>();
    config.show_timing = j["show_timing"].get<bool>();

    return config;
}

OptionType Config::parse_option_type(const std::string& type_str) {
    if (type_str == "call") return OptionType::Call;
    if (type_str == "put") return OptionType::Put;
    throw std::runtime_error("Invalid option type: " + type_str);
}

} // namespace montecarlo 