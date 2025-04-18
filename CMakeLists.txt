cmake_minimum_required(VERSION 3.10)
project(MonteCarloOptionPricing VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add nlohmann/json library
include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
    URL_HASH MD5=6bea5877b1541d353bd77bdfbbdb1dcf
)
FetchContent_MakeAvailable(json)

# Add CLI11 library for command-line parsing
find_package(CLI11 REQUIRED)

# Add Catch2 for testing
find_package(Catch2 REQUIRED)

# Add spdlog for logging
find_package(spdlog REQUIRED)

# Add executable
add_executable(MonteCarloOptionPricing
    src/main.cpp
    src/ConfigLoader.cpp
    src/BlackScholesModel.cpp
    src/OptionPricer.cpp
)

# Include directories
target_include_directories(MonteCarloOptionPricing PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(MonteCarloOptionPricing PRIVATE 
    Threads::Threads
    nlohmann_json::nlohmann_json
    CLI11::CLI11
    spdlog::spdlog
)

# Add tests
enable_testing()
add_executable(MonteCarloOptionPricingTests
    tests/main.cpp
    tests/ConfigLoaderTests.cpp
    tests/BlackScholesModelTests.cpp
    tests/OptionPricerTests.cpp
)

target_include_directories(MonteCarloOptionPricingTests PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(MonteCarloOptionPricingTests PRIVATE
    Catch2::Catch2WithMain
    nlohmann_json::nlohmann_json
    spdlog::spdlog
)

# Install targets
install(TARGETS MonteCarloOptionPricing
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/
    DESTINATION include
) 