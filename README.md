# Monte Carlo Option Pricing

A high-performance C++ implementation for option pricing using Monte Carlo simulation. This project provides a robust framework for pricing European options with configurable parameters and multiple output formats.

## Key Features

- Multi-threaded Monte Carlo simulation engine
- Support for both call and put options
- Configurable simulation parameters
- Multiple output formats (CSV, JSON, text)
- Comprehensive test suite
- Detailed logging system

## Requirements

- CMake 3.10+
- C++17 compiler
- vcpkg package manager

## Quick Start

1. Install dependencies:
```powershell
cd C:\
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
.\vcpkg install nlohmann-json:x64-windows catch2:x64-windows spdlog:x64-windows
```

2. Build the project:
```powershell
cd C:\Users\Kayla\MonteCarlo\MonteCarlo
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

## Usage

### Basic Example
```powershell
.\build\bin\MonteCarloOptionPricing.exe
```

### Advanced Usage
```powershell
.\MonteCarloOptionPricing.exe -o results.json --format json --simulations 1000000 --threads 4
```

### Command Line Options
| Option | Description |
|--------|-------------|
| `-c, --config` | Configuration file path (default: config.json) |
| `-n, --simulations` | Number of Monte Carlo simulations |
| `-t, --threads` | Number of parallel threads |
| `--type` | Option type (call/put) |
| `-S, --spot` | Initial stock price |
| `-K, --strike` | Strike price |
| `-r, --rate` | Risk-free rate |
| `-v, --volatility` | Volatility |
| `-T, --maturity` | Time to maturity (years) |
| `-p, --precision` | Output precision |
| `-o, --output` | Output file path |
| `-f, --format` | Output format (text/csv/json) |

## Testing

Run the test suite:
```powershell
cd build
ctest -C Debug --output-on-failure
```

## Project Structure

```
MonteCarlo/
├── include/          # Header files
├── src/             # Implementation files
├── tests/           # Test suite
└── CMakeLists.txt   # Build configuration
```

## Configuration

Example configuration file (`config.json`):
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

MIT License
