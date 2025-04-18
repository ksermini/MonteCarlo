#include "OptionPricer.h"
#include <thread>
#include <vector>
#include <cmath>

OptionPricer::OptionPricer(double S, double K, double r, double sigma, double T,
                          OptionType type, std::unique_ptr<IPricingModel> model)
    : S_(S), K_(K), r_(r), sigma_(sigma), T_(T), type_(type), model_(std::move(model)) {}

double OptionPricer::price(unsigned int num_simulations, unsigned int num_threads) {
    // Ensure at least one thread
    num_threads = std::max(1u, num_threads);
    
    // Calculate simulations per thread
    unsigned int sims_per_thread = num_simulations / num_threads;
    unsigned int remaining_sims = num_simulations % num_threads;
    
    // Vector to store thread results
    std::vector<double> thread_results(num_threads, 0.0);
    std::vector<std::thread> threads;
    
    // Launch threads
    unsigned int start = 0;
    for (unsigned int i = 0; i < num_threads; ++i) {
        unsigned int end = start + sims_per_thread + (i < remaining_sims ? 1 : 0);
        threads.emplace_back(&OptionPricer::simulate_range, this, start, end, 
                           std::ref(thread_results[i]));
        start = end;
    }
    
    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
    
    // Aggregate results
    double total_payoff = 0.0;
    for (double result : thread_results) {
        total_payoff += result;
    }
    
    // Calculate average payoff and discount
    double average_payoff = total_payoff / num_simulations;
    return average_payoff * std::exp(-r_ * T_);
}

void OptionPricer::simulate_range(unsigned int start, unsigned int end, double& result) {
    double sum_payoffs = 0.0;
    
    for (unsigned int i = start; i < end; ++i) {
        double ST = model_->simulate_price(S_, K_, r_, sigma_, T_);
        sum_payoffs += payoff(ST);
    }
    
    result = sum_payoffs;
}

double OptionPricer::payoff(double ST) const {
    switch (type_) {
        case OptionType::Call:
            return std::max(ST - K_, 0.0);
        case OptionType::Put:
            return std::max(K_ - ST, 0.0);
        default:
            return 0.0;
    }
} 