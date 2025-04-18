#include "OptionPricer.h"
#include <cmath>
#include <random>
#include <thread>
#include <chrono>

namespace montecarlo {

OptionPricer::OptionPricer(const BlackScholesModel& model,
                          unsigned int num_simulations,
                          unsigned int num_threads)
    : model_(model),
      num_simulations_(num_simulations),
      num_threads_(num_threads) {
}

PricingResult OptionPricer::price_option(const Payoff& payoff, double T) {
    auto start_time = std::chrono::high_resolution_clock::now();

    // Calculate number of simulations per thread
    unsigned int sims_per_thread = num_simulations_ / num_threads_;
    unsigned int remaining_sims = num_simulations_ % num_threads_;

    // Initialize results
    double sum_payoffs = 0.0;
    double sum_squared_payoffs = 0.0;
    std::mutex mutex;

    // Create and launch threads
    std::vector<std::thread> threads;
    unsigned int start_idx = 0;

    for (unsigned int i = 0; i < num_threads_; ++i) {
        unsigned int thread_sims = sims_per_thread + (i < remaining_sims ? 1 : 0);
        unsigned int end_idx = start_idx + thread_sims;

        threads.emplace_back([this, start_idx, end_idx, &sum_payoffs, &sum_squared_payoffs, &mutex, &payoff, T]() {
            double local_sum = 0.0;
            double local_sum_squared = 0.0;
            
            simulate_range(start_idx, end_idx, local_sum, local_sum_squared, payoff, T);

            // Update global sums with thread safety
            std::lock_guard<std::mutex> lock(mutex);
            sum_payoffs += local_sum;
            sum_squared_payoffs += local_sum_squared;
        });

        start_idx = end_idx;
    }

    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }

    // Calculate final results
    double mean_payoff = sum_payoffs / num_simulations_;
    double mean_squared_payoff = sum_squared_payoffs / num_simulations_;
    double variance = mean_squared_payoff - mean_payoff * mean_payoff;
    double standard_error = std::sqrt(variance / num_simulations_);

    // Apply discounting
    double discounted_price = mean_payoff * std::exp(-model_.get_risk_free_rate() * T);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto computation_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    return {discounted_price, standard_error, computation_time};
}

void OptionPricer::simulate_range(unsigned int start_idx,
                                unsigned int end_idx,
                                double& sum_payoffs,
                                double& sum_squared_payoffs,
                                const Payoff& payoff,
                                double T) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(0.0, 1.0);

    double S0 = model_.get_initial_price();
    double r = model_.get_risk_free_rate();
    double sigma = model_.get_volatility();
    double dt = T;

    for (unsigned int i = start_idx; i < end_idx; ++i) {
        double z = dist(gen);
        double S_T = S0 * std::exp((r - 0.5 * sigma * sigma) * dt + sigma * std::sqrt(dt) * z);
        
        double payoff_value = payoff.calculate(S_T);
        sum_payoffs += payoff_value;
        sum_squared_payoffs += payoff_value * payoff_value;
    }
}

} // namespace montecarlo 