#include "OptionPricer.h"
#include "BlackScholesModel.h"
#include "OptionType.h"
#include <iostream>
#include <iomanip>
#include <chrono>

int main() {
    try {
        // Option parameters
        double S = 100.0;      // Initial stock price
        double K = 100.0;      // Strike price
        double r = 0.05;       // Risk-free rate
        double sigma = 0.2;    // Volatility
        double T = 1.0;        // Time to maturity (1 year)
        
        // Simulation parameters
        unsigned int num_simulations = 1000000;
        unsigned int num_threads = std::thread::hardware_concurrency();
        
        // Create pricing model and pricer
        auto model = std::make_unique<BlackScholesModel>();
        OptionPricer pricer(S, K, r, sigma, T, OptionType::Call, std::move(model));
        
        // Time the pricing
        auto start = std::chrono::high_resolution_clock::now();
        double price = pricer.price(num_simulations, num_threads);
        auto end = std::chrono::high_resolution_clock::now();
        
        // Calculate duration
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // Output results
        std::cout << "Monte Carlo Option Pricing Results:" << std::endl;
        std::cout << "---------------------------------" << std::endl;
        std::cout << "Option Type: Call" << std::endl;
        std::cout << "Stock Price (S): " << S << std::endl;
        std::cout << "Strike Price (K): " << K << std::endl;
        std::cout << "Risk-free Rate (r): " << r << std::endl;
        std::cout << "Volatility (σ): " << sigma << std::endl;
        std::cout << "Time to Maturity (T): " << T << " years" << std::endl;
        std::cout << "Number of Simulations: " << num_simulations << std::endl;
        std::cout << "Number of Threads: " << num_threads << std::endl;
        std::cout << "Calculated Price: " << std::fixed << std::setprecision(4) << price << std::endl;
        std::cout << "Computation Time: " << duration.count() << " ms" << std::endl;
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
} 