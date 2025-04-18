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
#include "ResultExporter.h"

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

        // Simulation parameters
        unsigned int num_simulations = 0;
        unsigned int num_threads = 0;
        app.add_option("--simulations,-n", num_simulations, 
            "Number of Monte Carlo simulations (overrides config)")
            ->check(CLI::PositiveNumber);
        app.add_option("--threads,-t", num_threads, 
            "Number of threads for parallel computation (overrides config)")
            ->check(CLI::PositiveNumber);

        // Option parameters
        std::string option_type_str;
        double S = 0.0, K = 0.0, r = 0.0, sigma = 0.0, T = 0.0;
        app.add_option("--type", option_type_str, 
            "Option type (call/put) (overrides config)")
            ->check(CLI::IsMember({"call", "put"}));
        app.add_option("--spot,-S", S, 
            "Initial stock price (overrides config)")
            ->check(CLI::PositiveNumber);
        app.add_option("--strike,-K", K, 
            "Strike price (overrides config)")
            ->check(CLI::PositiveNumber);
        app.add_option("--rate,-r", r, 
            "Risk-free interest rate (overrides config)")
            ->check(CLI::NonNegativeNumber);
        app.add_option("--volatility,-v", sigma, 
            "Volatility (overrides config)")
            ->check(CLI::PositiveNumber);
        app.add_option("--maturity,-T", T, 
            "Time to maturity in years (overrides config)")
            ->check(CLI::PositiveNumber);

        // Output parameters
        int precision = -1;
        bool show_timing = true;
        std::string output_file;
        std::string output_format = "text";
        app.add_option("--precision,-p", precision, 
            "Number of decimal places in output (overrides config)")
            ->check(CLI::NonNegativeNumber);
        app.add_flag("--timing{true},--no-timing{false}", show_timing, 
            "Show computation timing (overrides config)");
        app.add_option("--output,-o", output_file, 
            "Output file path (if not specified, results are printed to console)");
        app.add_option("--format,-f", output_format, 
            "Output format (text/csv/json)")
            ->check(CLI::IsMember({"text", "csv", "json"}));

        // Additional options
        bool validate_config = false;
        app.add_flag("--validate-config", validate_config, 
            "Validate configuration file and exit");
        bool dry_run = false;
        app.add_flag("--dry-run", dry_run, 
            "Perform a dry run without actual pricing");

        // Parse command line
        CLI11_PARSE(app, argc, argv);

        // Load configuration
        montecarlo::Logger::info("Loading configuration from " + config_file);
        auto config = montecarlo::Config::load(config_file);
        montecarlo::Logger::info("Configuration loaded successfully");

        // Override config values if provided via command line
        if (num_simulations > 0) config.num_simulations = num_simulations;
        if (num_threads > 0) config.num_threads = num_threads;
        if (!option_type_str.empty()) {
            config.option_type = montecarlo::Config::parse_option_type(option_type_str);
        }
        if (S > 0.0) config.S = S;
        if (K > 0.0) config.K = K;
        if (r > 0.0) config.r = r;
        if (sigma > 0.0) config.sigma = sigma;
        if (T > 0.0) config.T = T;
        if (precision >= 0) config.precision = precision;
        config.show_timing = show_timing;

        // Validate config if requested
        if (validate_config) {
            montecarlo::Logger::info("Configuration validation successful");
            montecarlo::Logger::shutdown();
            return 0;
        }

        // Dry run if requested
        if (dry_run) {
            montecarlo::Logger::info("Dry run completed successfully");
            montecarlo::Logger::shutdown();
            return 0;
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
        if (!output_file.empty()) {
            montecarlo::Logger::info("Exporting results to " + output_file);
            if (output_format == "csv") {
                montecarlo::ResultExporter::export_to_csv(output_file, result, config);
            } else if (output_format == "json") {
                montecarlo::ResultExporter::export_to_json(output_file, result, config);
            } else {
                montecarlo::ResultExporter::export_to_text(output_file, result, config);
            }
            montecarlo::Logger::info("Results exported successfully");
        } else {
            // Print to console
            montecarlo::Logger::info("Option Price: " + std::to_string(result.price));
            montecarlo::Logger::info("Standard Error: " + std::to_string(result.standard_error));
            if (config.show_timing) {
                montecarlo::Logger::info("Computation Time: " + 
                    std::to_string(result.computation_time.count()) + " ms");
            }
        }

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