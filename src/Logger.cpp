#include "Logger.h"

namespace montecarlo {

std::ofstream Logger::log_file_;
std::mutex Logger::log_mutex_;

} // namespace montecarlo 