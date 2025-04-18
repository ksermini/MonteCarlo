cmake_minimum_required(VERSION 3.10)
project(MonteCarloOptionPricing VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(MonteCarloOptionPricing
    main.cpp
    OptionPricer.cpp
    BlackScholesModel.cpp
)

# Include directories
target_include_directories(MonteCarloOptionPricing PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Enable threading
find_package(Threads REQUIRED)
target_link_libraries(MonteCarloOptionPricing PRIVATE Threads::Threads) 