#pragma once

namespace montecarlo {

/**
 * @brief Enumeration of option types
 * 
 * This enum defines the different types of options that can be priced
 * by the Monte Carlo Option Pricing Engine.
 */
enum class OptionType {
    Call,   ///< Call option
    Put     ///< Put option
};

} // namespace montecarlo 