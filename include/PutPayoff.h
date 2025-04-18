#pragma once

#include "Payoff.h"

namespace montecarlo {

/**
 * @brief Concrete implementation of Payoff for put options
 */
class PutPayoff : public Payoff {
public:
    /**
     * @brief Construct a new Put Payoff object
     * 
     * @param K Strike price
     */
    explicit PutPayoff(double K) : K_(K) {}

    /**
     * @brief Calculate the payoff for a put option
     * 
     * @param S_T Terminal stock price
     * @return double max(K - S_T, 0)
     */
    double calculate(double S_T) const override {
        return std::max(K_ - S_T, 0.0);
    }

    /**
     * @brief Create a copy of this payoff object
     * 
     * @return std::unique_ptr<Payoff> A new PutPayoff with the same strike price
     */
    std::unique_ptr<Payoff> clone() const override {
        return std::make_unique<PutPayoff>(K_);
    }

private:
    double K_;  // Strike price
};

} // namespace montecarlo 