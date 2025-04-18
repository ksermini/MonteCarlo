#pragma once

/**
 * @brief Abstract interface for pricing models
 * 
 * This interface defines the contract for all pricing models that can be used
 * with the Monte Carlo Option Pricing Engine.
 */
class IPricingModel {
public:
    virtual ~IPricingModel() = default;

    /**
     * @brief Simulates a single price path
     * 
     * @param S Initial asset price
     * @param K Strike price
     * @param r Risk-free interest rate
     * @param sigma Volatility
     * @param T Time to maturity
     * @return double Simulated price at maturity
     */
    virtual double simulate_price(double S, double K, double r, double sigma, double T) const = 0;
}; 