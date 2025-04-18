#include "OptionPricer.h"
#include "BlackScholesModel.h"
#include "CallPayoff.h"
#include "PutPayoff.h"
#include <catch2/catch_test_macros.hpp>
#include <cmath>

namespace montecarlo {

// Helper function to calculate standard normal CDF
double normal_cdf(double x) {
    return 0.5 * (1.0 + std::erf(x / std::sqrt(2.0)));
}

// Helper function to calculate Black-Scholes price
double black_scholes_price(double S, double K, double r, double sigma, double T, bool is_call) {
    double d1 = (std::log(S/K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    double d2 = d1 - sigma * std::sqrt(T);
    
    if (is_call) {
        return S * normal_cdf(d1) - K * std::exp(-r * T) * normal_cdf(d2);
    } else {
        return K * std::exp(-r * T) * normal_cdf(-d2) - S * normal_cdf(-d1);
    }
}

TEST_CASE("OptionPricer initialization", "[OptionPricer]") {
    BlackScholesModel model(100.0, 0.05, 0.2);
    unsigned int num_simulations = 1000000;
    unsigned int num_threads = 4;
    
    OptionPricer pricer(model, num_simulations, num_threads);
    
    // No direct getters, but we can verify the pricing works
    CallPayoff payoff(100.0);
    auto result = pricer.price_option(payoff, 1.0);
    REQUIRE(result.price > 0.0);
    REQUIRE(result.standard_error > 0.0);
    REQUIRE(result.computation_time.count() > 0);
}

TEST_CASE("OptionPricer convergence", "[OptionPricer]") {
    BlackScholesModel model(100.0, 0.05, 0.2);
    unsigned int num_threads = 4;
    
    // Test convergence with increasing number of simulations
    std::vector<unsigned int> num_sims = {10000, 100000, 1000000};
    std::vector<double> prices;
    std::vector<double> standard_errors;
    
    CallPayoff payoff(100.0);
    double T = 1.0;
    
    for (auto n : num_sims) {
        OptionPricer pricer(model, n, num_threads);
        auto result = pricer.price_option(payoff, T);
        prices.push_back(result.price);
        standard_errors.push_back(result.standard_error);
    }
    
    // Check that standard error decreases with more simulations
    for (size_t i = 1; i < standard_errors.size(); ++i) {
        REQUIRE(standard_errors[i] < standard_errors[i-1]);
    }
    
    // Compare with analytical Black-Scholes price
    double bs_price = black_scholes_price(100.0, 100.0, 0.05, 0.2, 1.0, true);
    REQUIRE(std::abs(prices.back() - bs_price) < 3 * standard_errors.back());
}

TEST_CASE("OptionPricer edge cases", "[OptionPricer]") {
    SECTION("Zero volatility") {
        BlackScholesModel model(100.0, 0.05, 0.0);
        OptionPricer pricer(model, 100000, 4);
        CallPayoff payoff(100.0);
        auto result = pricer.price_option(payoff, 1.0);
        double expected = std::max(100.0 * std::exp(0.05 * 1.0) - 100.0, 0.0);
        REQUIRE(std::abs(result.price - expected) < 0.01);
    }
    
    SECTION("Zero interest rate") {
        BlackScholesModel model(100.0, 0.0, 0.2);
        OptionPricer pricer(model, 100000, 4);
        CallPayoff payoff(100.0);
        auto result = pricer.price_option(payoff, 1.0);
        REQUIRE(result.price > 0.0);
    }
    
    SECTION("Zero time to maturity") {
        BlackScholesModel model(100.0, 0.05, 0.2);
        OptionPricer pricer(model, 100000, 4);
        CallPayoff payoff(100.0);
        auto result = pricer.price_option(payoff, 0.0);
        REQUIRE(std::abs(result.price - std::max(100.0 - 100.0, 0.0)) < 0.01);
    }
}

} // namespace montecarlo 