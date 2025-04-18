#include "ResultExporter.h"
#include <fstream>
#include <iomanip>
#include <ctime>

namespace montecarlo {

void ResultExporter::export_to_csv(const std::string& filename,
                                 const PricingResult& result,
                                 const Config& config) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }

    // Write header
    file << "Parameter,Value\n";
    
    // Write simulation parameters
    file << "num_simulations," << config.num_simulations << "\n";
    file << "num_threads," << config.num_threads << "\n";
    
    // Write option parameters
    file << "option_type," << (config.option_type == OptionType::Call ? "call" : "put") << "\n";
    file << "S," << config.S << "\n";
    file << "K," << config.K << "\n";
    file << "r," << config.r << "\n";
    file << "sigma," << config.sigma << "\n";
    file << "T," << config.T << "\n";
    
    // Write results
    file << "price," << std::setprecision(config.precision) << result.price << "\n";
    file << "standard_error," << std::setprecision(config.precision) << result.standard_error << "\n";
    file << "computation_time_ms," << result.computation_time.count() << "\n";
}

void ResultExporter::export_to_json(const std::string& filename,
                                  const PricingResult& result,
                                  const Config& config) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }

    nlohmann::json j;
    
    // Add simulation parameters
    j["simulation"] = {
        {"num_simulations", config.num_simulations},
        {"num_threads", config.num_threads}
    };
    
    // Add option parameters
    j["option"] = {
        {"type", config.option_type == OptionType::Call ? "call" : "put"},
        {"parameters", {
            {"S", config.S},
            {"K", config.K},
            {"r", config.r},
            {"sigma", config.sigma},
            {"T", config.T}
        }}
    };
    
    // Add results
    j["results"] = {
        {"price", result.price},
        {"standard_error", result.standard_error},
        {"computation_time_ms", result.computation_time.count()}
    };
    
    // Add metadata
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    char time_str[26];
    ctime_s(time_str, sizeof(time_str), &time);
    j["metadata"] = {
        {"timestamp", time_str},
        {"precision", config.precision}
    };
    
    file << std::setw(4) << j << std::endl;
}

void ResultExporter::export_to_text(const std::string& filename,
                                  const PricingResult& result,
                                  const Config& config) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }

    // Get current time
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    char time_str[26];
    ctime_s(time_str, sizeof(time_str), &time);
    
    file << "Monte Carlo Option Pricing Results\n";
    file << "=================================\n\n";
    
    file << "Timestamp: " << time_str << "\n";
    
    file << "Simulation Parameters:\n";
    file << "---------------------\n";
    file << "Number of simulations: " << config.num_simulations << "\n";
    file << "Number of threads: " << config.num_threads << "\n\n";
    
    file << "Option Parameters:\n";
    file << "-----------------\n";
    file << "Type: " << (config.option_type == OptionType::Call ? "Call" : "Put") << "\n";
    file << "Spot Price (S): " << config.S << "\n";
    file << "Strike Price (K): " << config.K << "\n";
    file << "Risk-free Rate (r): " << config.r << "\n";
    file << "Volatility (σ): " << config.sigma << "\n";
    file << "Time to Maturity (T): " << config.T << "\n\n";
    
    file << "Results:\n";
    file << "--------\n";
    file << "Option Price: " << std::setprecision(config.precision) << result.price << "\n";
    file << "Standard Error: " << std::setprecision(config.precision) << result.standard_error << "\n";
    file << "Computation Time: " << result.computation_time.count() << " ms\n";
}

} // namespace montecarlo 