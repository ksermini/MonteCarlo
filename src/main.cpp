#include <iostream>
#include <string>
#include <memory>
#include "Config.h"
#include "BlackScholesModel.h"
#include "OptionPricer.h"
#include "Logger.h"
#include "CLI/CLI.hpp"
#include "CallPayoff.h"
#include "PutPayoff.h"

int main(int argc, char* argv[]) {
    try {
        // Initialize logger
        montecarlo::Logger::init();

        // Parse command line arguments
        CLI::App app{"Monte Carlo Option Pricing Engine"};

        // Configuration file path
        std::string config_file = "config.json";
        app.add_option("-c,--config", config_file, "Path to configuration file")
            ->check(CLI::ExistingFile);

        // Override options
        double S = 0.0, K = 0.0, r = 0.0, sigma = 0.0, T = 0.0;
        unsigned int num_simulations = 0, num_threads = 0;
        std::string option_type_str;

        app.add_option("--initial-price", S, "Initial stock price");
        app.add_option("--strike-price", K, "Strike price");
        app.add_option("--risk-free-rate", r, "Risk-free interest rate");
        app.add_option("--volatility", sigma, "Volatility");
        app.add_option("--time-to-maturity", T, "Time to maturity");
        app.add_option("--num-simulations", num_simulations, "Number of Monte Carlo simulations");
        app.add_option("--num-threads", num_threads, "Number of threads for parallel computation");
        app.add_option("--option-type", option_type_str, "Option type (call/put)");

        // Parse command line
        CLI11_PARSE(app, argc, argv);

        // Load configuration
        montecarlo::Logger::info("Loading configuration from " + config_file);
        auto config = montecarlo::Config::load(config_file);
        montecarlo::Logger::info("Configuration loaded successfully");

        // Override config values if provided via command line
        if (S > 0.0) config.S = S;
        if (K > 0.0) config.K = K;
        if (r > 0.0) config.r = r;
        if (sigma > 0.0) config.sigma = sigma;
        if (T > 0.0) config.T = T;
        if (num_simulations > 0) config.num_simulations = num_simulations;
        if (num_threads > 0) config.num_threads = num_threads;
        if (!option_type_str.empty()) {
            config.option_type = montecarlo::Config::parse_option_type(option_type_str);
        }

        // Create model
        montecarlo::Logger::info("Creating Black-Scholes model...");
        auto model = std::make_unique<BlackScholesModel>(
            config.S,
            config.r,
            config.sigma
        );
        montecarlo::Logger::info("Model created successfully");

        // Create pricer
        montecarlo::Logger::info("Creating option pricer...");
        montecarlo::OptionPricer pricer(
            *model,
            config.num_simulations,
            config.num_threads
        );
        montecarlo::Logger::info("Pricer created successfully");

        // Create appropriate payoff strategy
        std::unique_ptr<montecarlo::Payoff> payoff;
        if (config.option_type == montecarlo::OptionType::Call) {
            payoff = std::make_unique<montecarlo::CallPayoff>(config.K);
        } else {
            payoff = std::make_unique<montecarlo::PutPayoff>(config.K);
        }

        // Price the option
        montecarlo::Logger::info("Calculating option price...");
        auto result = pricer.price_option(*payoff, config.T);

        // Output results
        montecarlo::Logger::info("Option Price: " + std::to_string(result.price));
        montecarlo::Logger::info("Standard Error: " + std::to_string(result.standard_error));
        montecarlo::Logger::info("Computation Time: " + std::to_string(result.computation_time.count()) + " ms");

        // Cleanup
        montecarlo::Logger::shutdown();
        return 0;

    } catch (const montecarlo::ConfigError& e) {
        montecarlo::Logger::error("Configuration error: " + std::string(e.what()));
        montecarlo::Logger::shutdown();
        return 1;
    } catch (const std::exception& e) {
        montecarlo::Logger::error("Unexpected error: " + std::string(e.what()));
        montecarlo::Logger::shutdown();
        return 1;
    }
} 