#include "OptionPricer.h"
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>

namespace montecarlo {

OptionPricer::OptionPricer(const BlackScholesModel& model, 
                          unsigned int num_simulations, 
                          unsigned int num_threads)
    : model_(model)
    , num_simulations_(num_simulations)
    , num_threads_(num_threads)
{
}

PricingResult OptionPricer::price_option(OptionType type, double K, double T) {
    auto start_time = std::chrono::high_resolution_clock::now();

    // Calculate number of simulations per thread
    unsigned int sims_per_thread = num_simulations_ / num_threads_;
    unsigned int remaining_sims = num_simulations_ % num_threads_;

    // Results storage
    std::vector<double> thread_results(num_threads_, 0.0);
    std::vector<double> thread_sum_squares(num_threads_, 0.0);
    std::vector<std::thread> threads;

    // Launch threads
    for (unsigned int i = 0; i < num_threads_; ++i) {
        unsigned int start = i * sims_per_thread;
        unsigned int end = start + sims_per_thread + (i == num_threads_ - 1 ? remaining_sims : 0);

        threads.emplace_back([this, start, end, &thread_results, &thread_sum_squares, type, K, T, i]() {
            simulate_range(start, end, thread_results[i], thread_sum_squares[i], type, K, T);
        });
    }

    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }

    // Aggregate results
    double total_payoff = std::accumulate(thread_results.begin(), thread_results.end(), 0.0);
    double total_sum_squares = std::accumulate(thread_sum_squares.begin(), thread_sum_squares.end(), 0.0);

    // Calculate final price and standard error
    double discount_factor = std::exp(-model_.get_risk_free_rate() * T);
    double price = (total_payoff / num_simulations_) * discount_factor;
    
    double variance = (total_sum_squares / num_simulations_) - std::pow(total_payoff / num_simulations_, 2);
    double standard_error = std::sqrt(variance / num_simulations_) * discount_factor;

    auto end_time = std::chrono::high_resolution_clock::now();
    auto computation_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    return PricingResult{price, standard_error, computation_time};
}

void OptionPricer::simulate_range(unsigned int start, unsigned int end,
                                double& result, double& sum_squares,
                                OptionType type, double K, double T) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> dist(0.0, 1.0);

    for (unsigned int i = start; i < end; ++i) {
        double ST = model_.simulate_price(model_.get_initial_price(), K, model_.get_risk_free_rate(), model_.get_volatility(), T);
        double payoff_value = payoff(ST, type, K);
        result += payoff_value;
        sum_squares += payoff_value * payoff_value;
    }
}

double OptionPricer::payoff(double ST, OptionType type, double K) {
    if (type == OptionType::Call) {
        return std::max(ST - K, 0.0);
    } else {
        return std::max(K - ST, 0.0);
    }
}

} // namespace montecarlo 