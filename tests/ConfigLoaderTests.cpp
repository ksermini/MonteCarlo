#include <catch2/catch_test_macros.hpp>
#include "montecarlo/ConfigLoader.h"
#include <fstream>
#include <filesystem>

TEST_CASE("ConfigLoader loads valid configuration", "[config]") {
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
                "type": "Call",
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
    REQUIRE(config.num_simulations == 1000000);
    REQUIRE(config.num_threads == std::thread::hardware_concurrency());
    REQUIRE(config.option_type == montecarlo::OptionType::Call);
    REQUIRE(config.S == 100.0);
    REQUIRE(config.K == 100.0);
    REQUIRE(config.r == 0.05);
    REQUIRE(config.sigma == 0.2);
    REQUIRE(config.T == 1.0);
    REQUIRE(config.precision == 4);
    REQUIRE(config.show_timing == true);

    // Clean up
    std::filesystem::remove(config_file);
}

TEST_CASE("ConfigLoader validates parameters", "[config]") {
    // Create temporary config file with invalid values
    std::string config_file = "test_config.json";
    {
        std::ofstream file(config_file);
        file << R"({
            "simulation": {
                "num_simulations": 0,
                "num_threads": 0
            },
            "option": {
                "type": "Call",
                "parameters": {
                    "S": -100.0,
                    "K": -100.0,
                    "r": -0.05,
                    "sigma": -0.2,
                    "T": -1.0
                }
            },
            "output": {
                "precision": -4,
                "show_timing": true
            }
        })";
    }

    // Try to load config and expect exceptions
    REQUIRE_THROWS_AS(montecarlo::Config::load(config_file), montecarlo::ConfigError);

    // Clean up
    std::filesystem::remove(config_file);
}

TEST_CASE("ConfigLoader handles CLI overrides", "[config]") {
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
                "type": "Call",
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

    // Create CLI options
    montecarlo::Config::CLIOptions cli_options;
    cli_options.num_simulations = 2000000;
    cli_options.option_type = "Put";
    cli_options.S = 150.0;

    // Load config with CLI overrides
    auto config = montecarlo::Config::load(config_file, cli_options);

    // Check overridden values
    REQUIRE(config.num_simulations == 2000000);
    REQUIRE(config.option_type == montecarlo::OptionType::Put);
    REQUIRE(config.S == 150.0);

    // Check non-overridden values
    REQUIRE(config.K == 100.0);
    REQUIRE(config.r == 0.05);
    REQUIRE(config.sigma == 0.2);
    REQUIRE(config.T == 1.0);

    // Clean up
    std::filesystem::remove(config_file);
} 