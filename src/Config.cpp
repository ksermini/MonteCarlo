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
    config.num_simulations = j["simulation"]["num_simulations"].get<unsigned int>();
    if (j["simulation"]["num_threads"].get<std::string>() == "auto") {
        config.num_threads = std::thread::hardware_concurrency();
    } else {
        config.num_threads = j["simulation"]["num_threads"].get<unsigned int>();
    }

    // Load option parameters
    config.option_type = parse_option_type(j["option"]["type"].get<std::string>());
    config.S = j["option"]["parameters"]["S"].get<double>();
    config.K = j["option"]["parameters"]["K"].get<double>();
    config.r = j["option"]["parameters"]["r"].get<double>();
    config.sigma = j["option"]["parameters"]["sigma"].get<double>();
    config.T = j["option"]["parameters"]["T"].get<double>();

    // Load output parameters
    config.precision = j["output"]["precision"].get<int>();
    config.show_timing = j["output"]["show_timing"].get<bool>();

    return config;
}

OptionType Config::parse_option_type(const std::string& type_str) {
    if (type_str == "call") return OptionType::Call;
    if (type_str == "put") return OptionType::Put;
    throw std::runtime_error("Invalid option type: " + type_str);
}

} // namespace montecarlo 