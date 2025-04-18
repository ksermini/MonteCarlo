#pragma once
#include "BlackScholesModel.h"
#include "OptionType.h"
#include <chrono>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <memory>
#include "Payoff.h"

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
     * @param model Reference to the pricing model
     * @param num_simulations Number of Monte Carlo simulations
     * @param num_threads Number of threads for parallel computation
     */
    OptionPricer(const BlackScholesModel& model, 
                unsigned int num_simulations,
                unsigned int num_threads);

    /**
     * @brief Price an option using Monte Carlo simulation
     * 
     * @param payoff The payoff strategy to use
     * @param T Time to maturity
     * @return PricingResult The pricing result including price, standard error, and computation time
     */
    PricingResult price_option(const Payoff& payoff, double T);

private:
    // Model reference
    const BlackScholesModel& model_;
    
    // Simulation parameters
    unsigned int num_simulations_;
    unsigned int num_threads_;

    /**
     * @brief Simulate a range of paths and accumulate results
     * 
     * @param start_idx Starting index of the simulation range
     * @param end_idx Ending index of the simulation range
     * @param sum_payoffs Reference to accumulate sum of payoffs
     * @param sum_squared_payoffs Reference to accumulate sum of squared payoffs
     * @param payoff The payoff strategy to use
     * @param T Time to maturity
     */
    void simulate_range(unsigned int start_idx,
                       unsigned int end_idx,
                       double& sum_payoffs,
                       double& sum_squared_payoffs,
                       const Payoff& payoff,
                       double T);

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