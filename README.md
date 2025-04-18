# Monte Carlo Option Pricing Engine

A high-performance C++ implementation of a Monte Carlo simulation engine for pricing European options. This project demonstrates advanced C++ programming techniques, multithreading, and quantitative finance concepts.

## Features

- European Call and Put option pricing
- Black-Scholes model implementation
- Multithreaded Monte Carlo simulation
- Thread-safe random number generation
- Clean, extensible architecture
- Performance-optimized implementation

## Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.10 or higher
- Threading support in your C++ standard library

## Building the Project

1. Clone the repository:
```bash
git clone <repository-url>
cd MonteCarloOptionPricing
```

2. Create a build directory:
```bash
mkdir build
cd build
```

3. Configure and build:
```bash
cmake ..
cmake --build .
```

## Running the Program

After building, run the executable:
```bash
./MonteCarloOptionPricing
```

The program will run a Monte Carlo simulation with the following default parameters:
- Initial stock price (S): 100.0
- Strike price (K): 100.0
- Risk-free rate (r): 0.05 (5%)
- Volatility (σ): 0.2 (20%)
- Time to maturity (T): 1.0 year
- Number of simulations: 1,000,000
- Number of threads: System's hardware concurrency

## Project Structure

```
MonteCarloOptionPricing/
├── CMakeLists.txt          # Build configuration
├── main.cpp                # Program entry point
├── IPricingModel.h         # Abstract pricing model interface
├── BlackScholesModel.h     # Black-Scholes model header
├── BlackScholesModel.cpp   # Black-Scholes model implementation
├── OptionType.h            # Option type enumeration
├── OptionPricer.h          # Main pricing engine header
└── OptionPricer.cpp        # Main pricing engine implementation
```

## Key Components

### IPricingModel
Abstract interface defining the contract for pricing models. Implementations must provide a `simulate_price` method that generates a single price path.

### BlackScholesModel
Implementation of the Black-Scholes model using geometric Brownian motion. Features thread-safe random number generation.

### OptionPricer
Main class coordinating the Monte Carlo simulation process:
- Manages multithreading
- Aggregates simulation results
- Calculates final option prices

### OptionType
Type-safe enumeration for option types (Call/Put).

## Performance Considerations

- Uses thread-local random number generators to avoid contention
- Implements efficient workload distribution across threads
- Minimizes memory allocations during simulation
- Uses modern C++ features for optimal performance

## Extending the Project

To add a new pricing model:
1. Create a new class implementing `IPricingModel`
2. Implement the `simulate_price` method
3. Use the new model in `OptionPricer`

Example:
```cpp
class NewModel : public IPricingModel {
    double simulate_price(double S, double K, double r, double sigma, double T) const override {
        // Implement your pricing logic
    }
};
```


## Author

Kayla Sermini