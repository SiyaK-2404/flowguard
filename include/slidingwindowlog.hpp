#pragma once

#include "ratelimiter.hpp"

#include <unordered_map>
#include <deque>
#include <mutex>
#include <chrono>
#include <string>

class SlidingWindowLog : public RateLimiter
{
public:
    SlidingWindowLog(int limit,
                     std::chrono::seconds windowSize);

    bool allow(const std::string& clientId) override;

private:
    int limit_;

    std::chrono::seconds windowSize_;

    std::unordered_map<
        std::string,
        std::deque<std::chrono::steady_clock::time_point>
    > logs_;

    std::mutex mutex_;
};