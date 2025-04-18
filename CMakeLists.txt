cmake_minimum_required(VERSION 3.10)

# Set vcpkg toolchain file
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

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

# Set consistent runtime library for MSVC
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL$<$<CONFIG:Debug>:Debug>")
    add_compile_options(/MD$<$<CONFIG:Debug>:d>)
endif()

# Add CLI11
include(FetchContent)
FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.4.1
)
FetchContent_MakeAvailable(CLI11)

# Add Catch2
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.2
)
FetchContent_MakeAvailable(Catch2)

# Add executable
add_executable(MonteCarloOptionPricing
    src/main.cpp
    src/Config.cpp
    src/BlackScholesModel.cpp
    src/OptionPricer.cpp
    src/Logger.cpp
    src/ResultExporter.cpp
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
    tests/BlackScholesModelTests.cpp
    tests/OptionPricerTests.cpp
    src/Config.cpp
    src/BlackScholesModel.cpp
    src/OptionPricer.cpp
    src/Logger.cpp
    src/ResultExporter.cpp
)

target_include_directories(MonteCarloOptionPricingTests PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Catch2_SOURCE_DIR}/single_include
)

target_link_libraries(MonteCarloOptionPricingTests PRIVATE
    Threads::Threads
    Catch2::Catch2WithMain
)

# Add test cases
add_test(NAME ConfigLoaderTests COMMAND MonteCarloOptionPricingTests [ConfigLoaderTests])
add_test(NAME BlackScholesModelTests COMMAND MonteCarloOptionPricingTests [BlackScholesModel])
add_test(NAME OptionPricerTests COMMAND MonteCarloOptionPricingTests [OptionPricer])

# Install targets
install(TARGETS MonteCarloOptionPricing
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/
    DESTINATION include
) 