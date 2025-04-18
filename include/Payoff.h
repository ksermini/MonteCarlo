#pragma once

#include <memory>

namespace montecarlo {

/**
 * @brief Abstract base class for option payoff strategies
 * 
 * This class defines the interface for all payoff strategies in the Monte Carlo
 * option pricing engine. Concrete implementations will define specific payoff
 * functions for different types of options.
 */
class Payoff {
public:
    virtual ~Payoff() = default;

    /**
     * @brief Calculate the payoff for a given terminal stock price
     * 
     * @param S_T Terminal stock price
     * @return double Option payoff
     */
    virtual double calculate(double S_T) const = 0;

    /**
     * @brief Create a copy of the payoff object
     * 
     * @return std::unique_ptr<Payoff> A new copy of the payoff object
     */
    virtual std::unique_ptr<Payoff> clone() const = 0;
};

} // namespace montecarlo 