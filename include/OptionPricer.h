#pragma once
#include "BlackScholesModel.h"
#include "OptionType.h"
#include <chrono>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

namespace montecarlo {

struct PricingResult {
    double price;
    double standard_error;
    std::chrono::milliseconds computation_time;
};

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
     * @param model Reference to the Black-Scholes model
     * @param num_simulations Number of Monte Carlo simulations to run
     * @param num_threads Number of threads to use for parallel computation
     */
    OptionPricer(const BlackScholesModel& model, unsigned int num_simulations, unsigned int num_threads);

    /**
     * @brief Calculate the option price using Monte Carlo simulation
     * 
     * @param type Option type (Call/Put)
     * @param K Strike price
     * @param T Time to maturity
     * @return PricingResult The calculated option price, standard error, and computation time
     */
    PricingResult price_option(OptionType type, double K, double T);

private:
    // Model reference
    const BlackScholesModel& model_;
    
    // Simulation parameters
    unsigned int num_simulations_;
    unsigned int num_threads_;

    /**
     * @brief Simulate a range of price paths
     * 
     * @param start Starting simulation index
     * @param end Ending simulation index
     * @param result Reference to store the sum of payoffs
     * @param sum_squares Reference to store the sum of squared payoffs
     * @param type Option type
     * @param K Strike price
     * @param T Time to maturity
     */
    void simulate_range(unsigned int start, unsigned int end, 
                       double& result, double& sum_squares,
                       OptionType type, double K, double T);

    /**
     * @brief Calculate the payoff for a given terminal price
     * 
     * @param ST Terminal price
     * @param type Option type
     * @param K Strike price
     * @return double Option payoff
     */
    static double payoff(double ST, OptionType type, double K);
};

} // namespace montecarlo 