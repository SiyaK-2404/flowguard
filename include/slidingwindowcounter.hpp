#pragma once

#include "ratelimiter.hpp"

#include <unordered_map>
#include <mutex>
#include <chrono>

class SlidingWindowCounter : public RateLimiter
{
public:
    SlidingWindowCounter(
        int limit,
        std::chrono::seconds windowSize);

    bool allow(const std::string& clientId) override;

private:

    struct Counter
    {
        int previousCount = 0;
        int currentCount = 0;

        std::chrono::steady_clock::time_point windowStart;
    };

    int limit_;

    std::chrono::seconds windowSize_;

    std::unordered_map<std::string, Counter> counters_;

    std::mutex mutex_;
};