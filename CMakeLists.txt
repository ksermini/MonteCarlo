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
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)

# Add CLI11 library for command-line parsing
FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.3.2
)
FetchContent_MakeAvailable(CLI11)

# Add Catch2 for testing
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.3.2
)
FetchContent_MakeAvailable(Catch2)

# Add spdlog for logging
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.11.0
)
FetchContent_MakeAvailable(spdlog)

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