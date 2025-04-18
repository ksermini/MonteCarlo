#include "OptionPricer.h"
#include "BlackScholesModel.h"
#include "OptionType.h"
#include "Config.h"
#include <iostream>
#include <iomanip>
#include <chrono>

int main(int argc, char* argv[]) {
    try {
        // Load configuration
        std::string config_file = "config.json";
        if (argc > 1) {
            config_file = argv[1];
        }
        
        Config config = Config::load(config_file);
        
        // Create pricing model and pricer
        auto model = std::make_unique<BlackScholesModel>();
        OptionPricer pricer(config.S, config.K, config.r, config.sigma, config.T, 
                          config.option_type, std::move(model));
        
        // Time the pricing
        auto start = std::chrono::high_resolution_clock::now();
        double price = pricer.price(config.num_simulations, config.num_threads);
        auto end = std::chrono::high_resolution_clock::now();
        
        // Calculate duration
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // Output results
        std::cout << "Monte Carlo Option Pricing Results:" << std::endl;
        std::cout << "---------------------------------" << std::endl;
        std::cout << "Option Type: " << (config.option_type == OptionType::Call ? "Call" : "Put") << std::endl;
        std::cout << "Stock Price (S): " << config.S << std::endl;
        std::cout << "Strike Price (K): " << config.K << std::endl;
        std::cout << "Risk-free Rate (r): " << config.r << std::endl;
        std::cout << "Volatility (σ): " << config.sigma << std::endl;
        std::cout << "Time to Maturity (T): " << config.T << " years" << std::endl;
        std::cout << "Number of Simulations: " << config.num_simulations << std::endl;
        std::cout << "Number of Threads: " << config.num_threads << std::endl;
        std::cout << "Calculated Price: " << std::fixed << std::setprecision(config.precision) << price << std::endl;
        
        if (config.show_timing) {
            std::cout << "Computation Time: " << duration.count() << " ms" << std::endl;
        }
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
} 