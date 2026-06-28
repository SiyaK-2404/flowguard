#include <iostream>
#include <iomanip>
#include <chrono>

#include "ratelimiter.hpp"
#include "tokenbucket.hpp"
#include "fixedwindow.hpp"
#include "slidingwindowlog.hpp"
#include "slidingwindowcounter.hpp"

const int NUM_REQUESTS = 100000;

void benchmark(const std::string& name,
               RateLimiter& limiter)
{
    int allowed = 0;
    int rejected = 0;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_REQUESTS; i++)
    {
        std::string user = "User" + std::to_string(i % 100);

        if (limiter.allow(user))
            allowed++;
        else
            rejected++;
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    double seconds = duration.count() / 1000.0;

    double throughput = (seconds > 0)
                        ? NUM_REQUESTS / seconds
                        : 0.0;

    double allowedPercent =
        (100.0 * allowed) / NUM_REQUESTS;

    double rejectedPercent =
        (100.0 * rejected) / NUM_REQUESTS;

    std::cout << std::fixed << std::setprecision(2);

    std::cout << "\n=============================================\n";
    std::cout << "Algorithm       : " << name << '\n';
    std::cout << "Requests        : " << NUM_REQUESTS << '\n';
    std::cout << "Allowed         : "
              << allowed
              << " (" << allowedPercent << "%)\n";

    std::cout << "Rejected        : "
              << rejected
              << " (" << rejectedPercent << "%)\n";

    std::cout << "Execution Time  : "
              << duration.count()
              << " ms\n";

    std::cout << "Throughput      : "
              << throughput
              << " req/sec\n";

    std::cout << "=============================================\n";
}

int main()
{
    TokenBucket tb(100, 20);

    FixedWindow fw(
        100,
        std::chrono::seconds(5));

    SlidingWindowLog swl(
        100,
        std::chrono::seconds(5));

    SlidingWindowCounter swc(
        100,
        std::chrono::seconds(5));

    benchmark("Token Bucket", tb);

    benchmark("Fixed Window", fw);

    benchmark("Sliding Window Log", swl);

    benchmark("Sliding Window Counter", swc);

    return 0;
}