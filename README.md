# Monte Carlo Option Pricing Engine

A high-performance C++ implementation of a Monte Carlo simulation engine for pricing European options. This project demonstrates advanced C++ programming techniques, multithreading, and quantitative finance concepts.

## Features

- European Call and Put option pricing
- Black-Scholes model implementation
- Multithreaded Monte Carlo simulation
- Thread-safe random number generation
- Clean, extensible architecture
- Performance-optimized implementation
- JSON-based configuration system

## Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.10 or higher
- Threading support in your C++ standard library
- Internet connection (for downloading nlohmann/json library)

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

## Configuration

The project uses a JSON configuration file (`config.json`) to set simulation parameters. You can modify this file to change:

- Simulation parameters (number of simulations, threads)
- Option parameters (type, strike price, volatility, etc.)
- Output settings (precision, timing display)

Example configuration:
```json
{
    "simulation": {
        "num_simulations": 1000000,
        "num_threads": "auto"
    },
    "option": {
        "type": "Call",
        "parameters": {
            "S": 100.0,
            "K": 100.0,
            "r": 0.05,
            "sigma": 0.2,
            "T": 1.0
        }
    },
    "output": {
        "precision": 4,
        "show_timing": true
    }
}
```

## Running the Program

After building, run the executable:
```bash
./MonteCarloOptionPricing [config_file]
```

If no configuration file is specified, it will use `config.json` by default.

## Project Structure

```
MonteCarloOptionPricing/
├── CMakeLists.txt          # Build configuration
├── config.json             # Default configuration
├── main.cpp                # Program entry point
├── Config.h                # Configuration class header
├── Config.cpp              # Configuration class implementation
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

### Config
Configuration management class that loads and parses JSON settings.

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

This repository is made available for reference only.
You may view and clone the code, but redistribution or reuse is prohibited without explicit permission.
