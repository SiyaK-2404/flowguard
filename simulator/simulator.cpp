#include <iostream>
#include <chrono>
#include <memory>
#include <cstdlib>

#include "ratelimiter.hpp"
#include "tokenbucket.hpp"
#include "fixedwindow.hpp"
#include "slidingwindowlog.hpp"
#include "slidingwindowcounter.hpp"

const int NUM_REQUESTS = 10000;
const int NUM_USERS = 100;

enum class TrafficPattern
{
    Uniform,
    Burst,
    Random
};

void simulate(const std::string& algorithm,
              RateLimiter& limiter,
              TrafficPattern pattern)
{
    int allowed = 0;
    int rejected = 0;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_REQUESTS; i++)
    {
        std::string user;

        switch (pattern)
        {
            case TrafficPattern::Uniform:
                user = "User" + std::to_string(i % NUM_USERS);
                break;

            case TrafficPattern::Burst:
                if (i < NUM_REQUESTS / 2)
                    user = "User0";
                else
                    user = "User" + std::to_string(i % NUM_USERS);
                break;

            case TrafficPattern::Random:
                user = "User" + std::to_string(rand() % NUM_USERS);
                break;
        }

        if (limiter.allow(user))
            allowed++;
        else
            rejected++;
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::string patternName;

    switch(pattern)
    {
        case TrafficPattern::Uniform:
            patternName = "Uniform";
            break;

        case TrafficPattern::Burst:
            patternName = "Burst";
            break;

        case TrafficPattern::Random:
            patternName = "Random";
            break;
    }

    std::cout << "\n--------------------------------------\n";
    std::cout << "Algorithm : " << algorithm << '\n';
    std::cout << "Pattern   : " << patternName << '\n';
    std::cout << "Allowed   : " << allowed << '\n';
    std::cout << "Rejected  : " << rejected << '\n';
    std::cout << "Time      : " << duration.count() << " ms\n";
}

int main()
{
    {
        TokenBucket tb(100, 20);
        simulate("Token Bucket", tb, TrafficPattern::Uniform);
    }

    {
        TokenBucket tb(100, 20);
        simulate("Token Bucket", tb, TrafficPattern::Burst);
    }

    {
        TokenBucket tb(100, 20);
        simulate("Token Bucket", tb, TrafficPattern::Random);
    }

    {
        FixedWindow fw(100, std::chrono::seconds(5));
        simulate("Fixed Window", fw, TrafficPattern::Uniform);
    }

    {
        FixedWindow fw(100, std::chrono::seconds(5));
        simulate("Fixed Window", fw, TrafficPattern::Burst);
    }

    {
        FixedWindow fw(100, std::chrono::seconds(5));
        simulate("Fixed Window", fw, TrafficPattern::Random);
    }

    {
        SlidingWindowLog swl(100, std::chrono::seconds(5));
        simulate("Sliding Window Log", swl, TrafficPattern::Uniform);
    }

    {
        SlidingWindowLog swl(100, std::chrono::seconds(5));
        simulate("Sliding Window Log", swl, TrafficPattern::Burst);
    }

    {
        SlidingWindowLog swl(100, std::chrono::seconds(5));
        simulate("Sliding Window Log", swl, TrafficPattern::Random);
    }

    {
        SlidingWindowCounter swc(100, std::chrono::seconds(5));
        simulate("Sliding Window Counter", swc, TrafficPattern::Uniform);
    }

    {
        SlidingWindowCounter swc(100, std::chrono::seconds(5));
        simulate("Sliding Window Counter", swc, TrafficPattern::Burst);
    }

    {
        SlidingWindowCounter swc(100, std::chrono::seconds(5));
        simulate("Sliding Window Counter", swc, TrafficPattern::Random);
    }

    return 0;
}