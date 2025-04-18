#pragma once

#include <fstream>
#include <string>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>
#include "Exceptions.h"

namespace montecarlo {

class Logger {
public:
    static void init(const std::string& log_file = "montecarlo.log") {
        try {
            log_file_.open(log_file);
            if (!log_file_.is_open()) {
                throw ConfigError("Failed to open log file: " + log_file);
            }
            log("INFO", "Logger initialized");
        } catch (const std::exception& e) {
            throw ConfigError("Log initialization failed: " + std::string(e.what()));
        }
    }

    static void shutdown() {
        if (log_file_.is_open()) {
            log_file_.close();
        }
    }

    static void info(const std::string& message) {
        log("INFO", message);
    }

    static void error(const std::string& message) {
        log("ERROR", message);
    }

    static void debug(const std::string& message) {
        log("DEBUG", message);
    }

private:
    static std::ofstream log_file_;
    static std::mutex log_mutex_;

    static void log(const std::string& level, const std::string& message) {
        std::lock_guard<std::mutex> lock(log_mutex_);
        
        // Get current time
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        
        // Format timestamp
        std::stringstream ss;
        std::tm local_time;
        localtime_s(&local_time, &time);
        ss << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");
        ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
        
        // Format log message
        std::string log_message = "[" + ss.str() + "] [" + level + "] " + message;
        
        // Write to console
        if (level == "ERROR") {
            std::cerr << log_message << std::endl;
        } else {
            std::cout << log_message << std::endl;
        }
        
        // Write to file
        if (log_file_.is_open()) {
            log_file_ << log_message << std::endl;
            log_file_.flush();
        }
    }
};

} // namespace montecarlo 