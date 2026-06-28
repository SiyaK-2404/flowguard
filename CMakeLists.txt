cmake_minimum_required(VERSION 3.16)

project(FlowGuard)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Library
add_library(FlowGuard
    src/tokenbucket.cpp
    src/fixedwindow.cpp
    src/slidingwindowlog.cpp
    src/slidingwindowcounter.cpp
)

# Benchmark executable
add_executable(benchmark
    benchmark/benchmark.cpp
)

target_link_libraries(benchmark FlowGuard)

# Simulator executable
add_executable(simulator
    simulator/simulator.cpp
)

target_link_libraries(simulator FlowGuard)