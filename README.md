# Monte Carlo Option Pricing

A C++ implementation of Monte Carlo simulation for option pricing, featuring parallel computation and multiple output formats.

## Features

- **Parallel Computation**: Utilizes multi-threading for faster option pricing
- **Multiple Output Formats**: Export results in CSV, JSON, or human-readable text
- **Configurable Parameters**: Adjust simulation parameters via configuration file or command line
- **Comprehensive Testing**: Unit tests for core functionality
- **Logging**: Detailed logging of the pricing process

## Prerequisites

- CMake (version 3.10 or higher)
- C++17 compatible compiler
- vcpkg for package management

## Installation

1. Install vcpkg:
```powershell
cd C:\
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
```

2. Install required packages:
```powershell
.\vcpkg install nlohmann-json:x64-windows
.\vcpkg install catch2:x64-windows
.\vcpkg install spdlog:x64-windows
```

3. Build the project:
```powershell
cd C:\Users\Kayla\MonteCarlo\MonteCarlo
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

## Usage

### Basic Usage

Run the program with default configuration:
```powershell
.\build\bin\MonteCarloOptionPricing.exe
```

### Command Line Options

- `-c, --config`: Path to configuration file (default: config.json)
- `-n, --simulations`: Number of Monte Carlo simulations
- `-t, --threads`: Number of threads for parallel computation
- `--type`: Option type (call/put)
- `-S, --spot`: Initial stock price
- `-K, --strike`: Strike price
- `-r, --rate`: Risk-free interest rate
- `-v, --volatility`: Volatility
- `-T, --maturity`: Time to maturity in years
- `-p, --precision`: Number of decimal places in output
- `--timing`: Show computation timing
- `-o, --output`: Output file path
- `-f, --format`: Output format (text/csv/json)
- `--validate-config`: Validate configuration file and exit
- `--dry-run`: Perform a dry run without actual pricing

### Output Formats

1. CSV Output:
```powershell
.\MonteCarloOptionPricing.exe -o results.csv --format csv
```

2. JSON Output:
```powershell
.\MonteCarloOptionPricing.exe -o results.json --format json
```

3. Text Output:
```powershell
.\MonteCarloOptionPricing.exe -o results.txt --format text
```

### Example with Multiple Options

```powershell
.\MonteCarloOptionPricing.exe -o results.json --format json --simulations 1000000 --threads 4 --precision 6
```

## Testing

Run all tests:
```powershell
cd build
ctest -C Debug --output-on-failure
```

Individual test cases:
- ConfigLoaderTests
- BlackScholesModelTests
- OptionPricerTests

## Project Structure

```
MonteCarlo/
├── include/
│   ├── BlackScholesModel.h
│   ├── Config.h
│   ├── Logger.h
│   ├── OptionPricer.h
│   ├── Payoff.h
│   ├── ResultExporter.h
│   └── ...
├── src/
│   ├── BlackScholesModel.cpp
│   ├── Config.cpp
│   ├── Logger.cpp
│   ├── main.cpp
│   ├── OptionPricer.cpp
│   ├── ResultExporter.cpp
│   └── ...
├── tests/
│   ├── BlackScholesModelTests.cpp
│   ├── ConfigLoaderTests.cpp
│   ├── OptionPricerTests.cpp
│   └── ...
└── CMakeLists.txt
```

## Configuration File Format

Example `config.json`:
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

## License

[Add your license information here]
