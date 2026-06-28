# FlowGuard

A thread-safe C++ rate limiting library implementing four industry-standard algorithms behind a unified API, along with benchmarking and traffic simulation tools for evaluating performance under different workloads.

---

## Overview

FlowGuard is a reusable C++ library designed for backend systems that need to control request rates. It provides multiple rate limiting algorithms through a common interface, allowing applications to switch implementations without changing their integration code.

The project also includes:

- A benchmarking suite for comparing algorithm performance
- A configurable traffic simulator with multiple workload patterns
- A modern CMake build system

---

## Features

- Thread-safe implementations using `std::mutex`
- Unified polymorphic interface (`RateLimiter`)
- Per-client rate limiting
- Four widely-used rate limiting algorithms
- Benchmark suite with throughput and latency metrics
- Traffic simulator supporting different traffic patterns
- Modern C++17 implementation
- CMake build support

---

## Implemented Algorithms

| Algorithm | Description | Time | Memory |
|------------|-------------|------|--------|
| **Token Bucket** | Allows short bursts while maintaining an average request rate. | O(1) | O(1) |
| **Fixed Window** | Uses a counter that resets after every fixed time window. | O(1) | O(1) |
| **Sliding Window Log** | Stores request timestamps for precise rate limiting. | O(1) amortized | O(n) |
| **Sliding Window Counter** | Uses weighted counts from consecutive windows to approximate a sliding window. | O(1) | O(1) |

---

## Choosing an Algorithm

| Algorithm | Best Use Case | Trade-off |
|------------|---------------|-----------|
| **Token Bucket** | APIs that should allow occasional bursts | Slightly more complex implementation |
| **Fixed Window** | Simple, high-performance rate limiting | Boundary burst problem |
| **Sliding Window Log** | Highest accuracy | Higher memory usage |
| **Sliding Window Counter** | Good balance between accuracy and memory | Approximate rather than exact |

---

## Project Structure

```text
FlowGuard/

├── include/
│   ├── RateLimiter.hpp
│   ├── TokenBucket.hpp
│   ├── FixedWindow.hpp
│   ├── SlidingWindowLog.hpp
│   └── SlidingWindowCounter.hpp
│
├── src/
│   ├── TokenBucket.cpp
│   ├── FixedWindow.cpp
│   ├── SlidingWindowLog.cpp
│   └── SlidingWindowCounter.cpp
│
├── benchmark/
│   └── benchmark.cpp
│
├── simulator/
│   └── simulator.cpp
│
├── CMakeLists.txt
└── README.md
```

---

## Architecture

```text
                    Application
                          │
                          ▼
                  RateLimiter Interface
                          │
       ┌──────────────────┼──────────────────┐
       │                  │                  │
       ▼                  ▼                  ▼
  TokenBucket       FixedWindow      SlidingWindowLog
                                                │
                                                ▼
                                     SlidingWindowCounter
```

Every algorithm derives from the common `RateLimiter` interface, allowing applications, benchmarks, and simulations to switch implementations without changing their logic.

---

## Example Usage

```cpp
#include "TokenBucket.hpp"

int main()
{
    TokenBucket limiter(100, 20);

    if (limiter.allow("client-123"))
    {
        // Process request
    }
    else
    {
        // Reject request
    }
}
```

---

## Benchmark

The benchmark executes each algorithm under the same workload and reports:

- Total requests
- Allowed requests
- Rejected requests
- Execution time
- Throughput

### Sample Output

<img width="342" height="872" alt="image" src="https://github.com/user-attachments/assets/c5ec4d08-8ddb-49c8-8bd2-095e4b0d277b" />
<img width="352" height="762" alt="image" src="https://github.com/user-attachments/assets/45563f6c-68d7-4e24-b30c-b7cbf5440ca5" />

---

## Traffic Simulator

The simulator evaluates the behavior of each algorithm under different request distributions.

### Supported Traffic Patterns

- **Uniform** – Requests are evenly distributed across clients.
- **Burst** – A single client generates a sudden spike in traffic.
- **Random** – Requests originate from randomly selected clients.

These scenarios demonstrate how different algorithms respond under realistic workloads.

<img width="357" height="960" alt="image" src="https://github.com/user-attachments/assets/5b33ce6a-4a48-4c71-b87f-c5ae29c3b53d" />
<img width="353" height="977" alt="image" src="https://github.com/user-attachments/assets/84d1b011-bc12-451f-9f7b-d2924b2e0c7d" />
<img width="353" height="977" alt="image" src="https://github.com/user-attachments/assets/da2b9bbc-3d97-4362-9ade-2811ff0e3a33" />

---

## Build Instructions

```bash
git clone <repository-url>

cd FlowGuard

mkdir build
cd build

cmake ..
cmake --build .
```

---

## Design Highlights

- Object-oriented architecture using runtime polymorphism
- Thread-safe implementations using `std::mutex`
- Independent per-client state with `std::unordered_map`
- Modular design allowing new algorithms to be added easily
- Benchmarking and simulation built on the common interface
- Modern C++17 and CMake project structure

---

## Future Improvements

- Unit tests
- Multi-threaded benchmarking
- JSON/YAML configuration support
- Additional algorithms (Leaky Bucket, GCRA)
- GitHub Actions CI pipeline
- Installable CMake package

