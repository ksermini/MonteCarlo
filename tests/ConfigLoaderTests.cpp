#include <fstream>
#include <filesystem>
#include <thread>
#include <iostream>
#include "Config.h"

void test_config_loading() {
    // Create temporary config file
    std::string config_file = "test_config.json";
    {
        std::ofstream file(config_file);
        file << R"({
            "simulation": {
                "num_simulations": 1000000,
                "num_threads": "auto"
            },
            "option": {
                "type": "call",
                "parameters": {
                    "S": 100.0,
                    "K": 100.0,
                    "r": 0.05,
                    "sigma": 0.2,
                    "T": 1.0
                }
            },
            "output": {
                "precision": 4,
                "show_timing": true
            }
        })";
    }

    // Load and validate config
    auto config = montecarlo::Config::load(config_file);

    // Check values
    if (config.num_simulations != 1000000) {
        throw std::runtime_error("num_simulations mismatch");
    }
    if (config.num_threads != std::thread::hardware_concurrency()) {
        throw std::runtime_error("num_threads mismatch");
    }
    if (config.option_type != montecarlo::OptionType::Call) {
        throw std::runtime_error("option_type mismatch");
    }
    if (config.S != 100.0) {
        throw std::runtime_error("S mismatch");
    }
    if (config.K != 100.0) {
        throw std::runtime_error("K mismatch");
    }
    if (config.r != 0.05) {
        throw std::runtime_error("r mismatch");
    }
    if (config.sigma != 0.2) {
        throw std::runtime_error("sigma mismatch");
    }
    if (config.T != 1.0) {
        throw std::runtime_error("T mismatch");
    }
    if (config.precision != 4) {
        throw std::runtime_error("precision mismatch");
    }
    if (!config.show_timing) {
        throw std::runtime_error("show_timing mismatch");
    }

    // Clean up
    std::filesystem::remove(config_file);
}

int main() {
    try {
        test_config_loading();
        std::cout << "All tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
} 