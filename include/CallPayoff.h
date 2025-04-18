#pragma once

#include "Payoff.h"

namespace montecarlo {

/**
 * @brief Concrete implementation of Payoff for call options
 */
class CallPayoff : public Payoff {
public:
    /**
     * @brief Construct a new Call Payoff object
     * 
     * @param K Strike price
     */
    explicit CallPayoff(double K) : K_(K) {}

    /**
     * @brief Calculate the payoff for a call option
     * 
     * @param S_T Terminal stock price
     * @return double max(S_T - K, 0)
     */
    double calculate(double S_T) const override {
        return std::max(S_T - K_, 0.0);
    }

    /**
     * @brief Create a copy of this payoff object
     * 
     * @return std::unique_ptr<Payoff> A new CallPayoff with the same strike price
     */
    std::unique_ptr<Payoff> clone() const override {
        return std::make_unique<CallPayoff>(K_);
    }

private:
    double K_;  // Strike price
};

} // namespace montecarlo 