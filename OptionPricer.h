#pragma once
#include "IPricingModel.h"
#include "OptionType.h"
#include <memory>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

/**
 * @brief Main class for option pricing using Monte Carlo simulation
 * 
 * This class coordinates the Monte Carlo simulation process, including
 * multithreading, result aggregation, and final price calculation.
 */
class OptionPricer {
public:
    /**
     * @brief Construct a new Option Pricer object
     * 
     * @param S Initial asset price
     * @param K Strike price
     * @param r Risk-free interest rate
     * @param sigma Volatility
     * @param T Time to maturity
     * @param type Option type (Call/Put)
     * @param model Pointer to the pricing model implementation
     */
    OptionPricer(double S, double K, double r, double sigma, double T,
                OptionType type, std::unique_ptr<IPricingModel> model);

    /**
     * @brief Calculate the option price using Monte Carlo simulation
     * 
     * @param num_simulations Number of Monte Carlo simulations to run
     * @param num_threads Number of threads to use for parallel computation
     * @return double The calculated option price
     */
    double price(unsigned int num_simulations, unsigned int num_threads);

private:
    // Option parameters
    double S_;      ///< Initial asset price
    double K_;      ///< Strike price
    double r_;      ///< Risk-free interest rate
    double sigma_;  ///< Volatility
    double T_;      ///< Time to maturity
    OptionType type_; ///< Option type

    // Pricing model
    std::unique_ptr<IPricingModel> model_;

    /**
     * @brief Simulate a range of price paths
     * 
     * @param start Starting simulation index
     * @param end Ending simulation index
     * @param result Reference to store the sum of payoffs
     */
    void simulate_range(unsigned int start, unsigned int end, double& result);

    /**
     * @brief Calculate the payoff for a given terminal price
     * 
     * @param ST Terminal price
     * @return double Option payoff
     */
    double payoff(double ST) const;
}; 