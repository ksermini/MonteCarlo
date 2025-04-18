#include "BlackScholesModel.h"
#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <random>

namespace montecarlo {

TEST_CASE("BlackScholesModel initialization", "[BlackScholesModel]") {
    double S = 100.0;
    double r = 0.05;
    double sigma = 0.2;
    
    BlackScholesModel model(S, r, sigma);
    
    REQUIRE(model.get_initial_price() == S);
    REQUIRE(model.get_risk_free_rate() == r);
    REQUIRE(model.get_volatility() == sigma);
}

TEST_CASE("BlackScholesModel price simulation", "[BlackScholesModel]") {
    double S = 100.0;
    double r = 0.05;
    double sigma = 0.2;
    double T = 1.0;
    
    BlackScholesModel model(S, r, sigma);
    
    // Test multiple simulations
    const int num_simulations = 10000;
    double sum = 0.0;
    double sum_squared = 0.0;
    
    for (int i = 0; i < num_simulations; ++i) {
        double ST = model.simulate_price(S, 0.0, r, sigma, T);
        sum += ST;
        sum_squared += ST * ST;
    }
    
    double mean = sum / num_simulations;
    double expected_mean = S * std::exp(r * T);
    double variance = (sum_squared / num_simulations) - mean * mean;
    double expected_variance = S * S * std::exp(2 * r * T) * 
                             (std::exp(sigma * sigma * T) - 1);
    
    // Check if mean and variance are within reasonable bounds
    REQUIRE(std::abs(mean - expected_mean) < 0.1 * expected_mean);
    REQUIRE(std::abs(variance - expected_variance) < 0.1 * expected_variance);
}

TEST_CASE("BlackScholesModel edge cases", "[BlackScholesModel]") {
    SECTION("Zero volatility") {
        BlackScholesModel model(100.0, 0.05, 0.0);
        double ST = model.simulate_price(100.0, 0.0, 0.05, 0.0, 1.0);
        double expected = 100.0 * std::exp(0.05 * 1.0);
        REQUIRE(ST == expected);
    }
    
    SECTION("Zero interest rate") {
        BlackScholesModel model(100.0, 0.0, 0.2);
        double ST = model.simulate_price(100.0, 0.0, 0.0, 0.2, 1.0);
        REQUIRE(ST > 0.0);
    }
    
    SECTION("Zero time to maturity") {
        BlackScholesModel model(100.0, 0.05, 0.2);
        double ST = model.simulate_price(100.0, 0.0, 0.05, 0.2, 0.0);
        REQUIRE(ST == 100.0);
    }
}

} // namespace montecarlo 