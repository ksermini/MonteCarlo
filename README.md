# Monte Carlo Option Pricing Engine

A C++ implementation of a Monte Carlo simulation engine for option pricing using the Black-Scholes model.

## Features

- Monte Carlo simulation for option pricing
- Black-Scholes model implementation
- Multi-threaded computation
- JSON-based configuration
- Command-line interface for parameter overrides
- Comprehensive logging
- Unit tests

## Prerequisites

- CMake 3.10 or higher
- C++17 compatible compiler
- Git

## Building the Project

1. Clone the repository:
```bash
git clone <repository-url>
cd MonteCarloOptionPricing
```

2. Create a build directory and run CMake:
```bash
mkdir build
cd build
cmake ..
```

3. Build the project:
```bash
cmake --build .
```

## Usage

### Basic Usage

Run the program with default configuration:
```bash
./bin/Release/MonteCarloOptionPricing
```

### Command Line Options

Override configuration parameters using command-line arguments:

```bash
./bin/Release/MonteCarloOptionPricing --initial-price 100.0 --strike-price 100.0 --risk-free-rate 0.05 --volatility 0.2 --time-to-maturity 1.0 --num-simulations 1000000 --num-threads 4 --option-type call
```

Available options:
- `-c, --config`: Path to configuration file (default: config.json)
- `--initial-price`: Initial stock price
- `--strike-price`: Strike price
- `--risk-free-rate`: Risk-free interest rate
- `--volatility`: Volatility
- `--time-to-maturity`: Time to maturity
- `--num-simulations`: Number of Monte Carlo simulations
- `--num-threads`: Number of threads for parallel computation
- `--option-type`: Option type (call/put)

### Configuration File

The program uses a JSON configuration file (default: config.json) with the following structure:

```json
{
    "num_simulations": 1000000,
    "num_threads": 4,
    "option_type": "call",
    "S": 100.0,
    "K": 100.0,
    "r": 0.05,
    "sigma": 0.2,
    "T": 1.0,
    "precision": 6,
    "show_timing": true
}
```

## Running Tests

To run the unit tests:
```bash
./bin/Release/MonteCarloOptionPricingTests
```

## License

[Your chosen license]

## Contributing

[Your contribution guidelines]
