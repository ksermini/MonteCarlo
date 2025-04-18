#pragma once
#include "montecarlo/IPricingModel.h"
#include <random>
#include <cmath>

namespace montecarlo {

/**
 * @brief Implementation of the Black-Scholes pricing model
 * 
 * This class implements the Black-Scholes model for option pricing using
 * Monte Carlo simulation with geometric Brownian motion.
 */
class BlackScholesModel : public IPricingModel {
public:
    BlackScholesModel() = default;
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

private:
    /**
     * @brief Generates a standard normal random variable
     * 
     * @return double Random value from standard normal distribution
     */
    double generate_normal_random() const;
};

} // namespace montecarlo 