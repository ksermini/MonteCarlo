#pragma once
#include "IPricingModel.h"
#include <random>
#include <cmath>

/**
 * @brief Implementation of the Black-Scholes pricing model
 * 
 * This class implements the Black-Scholes model for option pricing using
 * Monte Carlo simulation with geometric Brownian motion.
 */
class BlackScholesModel : public IPricingModel {
public:
    BlackScholesModel(double initial_price, double risk_free_rate, double volatility)
        : initial_price_(initial_price)
        , risk_free_rate_(risk_free_rate)
        , volatility_(volatility)
    {}

    ~BlackScholesModel() override = default;

    /**
     * @brief Simulates a price path using geometric Brownian motion
     * 
     * @param S Initial asset price
     * @param K Strike price
     * @param r Risk-free interest rate
     * @param sigma Volatility
     * @param T Time to maturity
     * @return double Simulated price at maturity
     */
    double simulate_price(double S, double K, double r, double sigma, double T) const override;

    // Getters
    double get_initial_price() const { return initial_price_; }
    double get_risk_free_rate() const { return risk_free_rate_; }
    double get_volatility() const { return volatility_; }

private:
    double initial_price_;
    double risk_free_rate_;
    double volatility_;

    /**
     * @brief Generates a standard normal random variable
     * 
     * @return double Random value from standard normal distribution
     */
    double generate_normal_random() const;
}; 