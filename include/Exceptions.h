#pragma once

#include <stdexcept>
#include <string>

namespace montecarlo {

class ConfigError : public std::runtime_error {
public:
    explicit ConfigError(const std::string& message) 
        : std::runtime_error("Configuration error: " + message) {}
};

class SimulationError : public std::runtime_error {
public:
    explicit SimulationError(const std::string& message) 
        : std::runtime_error("Simulation error: " + message) {}
};

class ValidationError : public std::runtime_error {
public:
    explicit ValidationError(const std::string& message) 
        : std::runtime_error("Validation error: " + message) {}
};

} // namespace montecarlo 