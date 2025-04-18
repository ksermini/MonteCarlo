#include "BlackScholesModel.h"
#include <random>
#include <cmath>

double BlackScholesModel::simulate_price(double S, double K, double r, double sigma, double T) const {
    // Generate random normal variable
    double Z = generate_normal_random();
    
    // Calculate drift and diffusion terms
    double drift = (r - 0.5 * sigma * sigma) * T;
    double diffusion = sigma * std::sqrt(T) * Z;
    
    // Calculate final price using geometric Brownian motion
    return S * std::exp(drift + diffusion);
}

double BlackScholesModel::generate_normal_random() const {
    // Use thread_local to ensure thread safety
    thread_local std::random_device rd;
    thread_local std::mt19937 gen(rd());
    thread_local std::normal_distribution<> dist(0.0, 1.0);
    
    return dist(gen);
} 