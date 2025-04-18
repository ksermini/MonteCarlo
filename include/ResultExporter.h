#pragma once
#include "OptionPricer.h"
#include "Config.h"
#include <string>
#include <fstream>
#include <memory>

namespace montecarlo {

/**
 * @brief Class for exporting simulation results to various formats
 */
class ResultExporter {
public:
    /**
     * @brief Export results to a CSV file
     * 
     * @param filename Output file path
     * @param result Pricing result
     * @param config Configuration used
     */
    static void export_to_csv(const std::string& filename, 
                            const PricingResult& result,
                            const Config& config);

    /**
     * @brief Export results to a JSON file
     * 
     * @param filename Output file path
     * @param result Pricing result
     * @param config Configuration used
     */
    static void export_to_json(const std::string& filename,
                             const PricingResult& result,
                             const Config& config);

    /**
     * @brief Export results to a human-readable text file
     * 
     * @param filename Output file path
     * @param result Pricing result
     * @param config Configuration used
     */
    static void export_to_text(const std::string& filename,
                             const PricingResult& result,
                             const Config& config);
};

} // namespace montecarlo 