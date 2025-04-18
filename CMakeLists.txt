cmake_minimum_required(VERSION 3.10)
project(MonteCarloOptionPricing VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party)

# Find required packages
find_package(Threads REQUIRED)

# Add CLI11
include(FetchContent)
FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.4.1
)
FetchContent_MakeAvailable(CLI11)

# Set consistent runtime library for MSVC
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    add_compile_options(/MT$<$<CONFIG:Debug>:d>)
endif()

# Add executable
add_executable(MonteCarloOptionPricing
    src/main.cpp
    src/Config.cpp
    src/BlackScholesModel.cpp
    src/OptionPricer.cpp
    src/Logger.cpp
)

# Include directories
target_include_directories(MonteCarloOptionPricing PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CLI11_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(MonteCarloOptionPricing PRIVATE 
    Threads::Threads
)

# Add tests
enable_testing()
add_executable(MonteCarloOptionPricingTests
    tests/ConfigLoaderTests.cpp
    src/Config.cpp
    src/Logger.cpp
)

target_include_directories(MonteCarloOptionPricingTests PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(MonteCarloOptionPricingTests PRIVATE
    Threads::Threads
)

# Install targets
install(TARGETS MonteCarloOptionPricing
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/
    DESTINATION include
) 