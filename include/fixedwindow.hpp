#pragma once

#include "ratelimiter.hpp"

#include <unordered_map>
#include <mutex>
#include <chrono>
#include <string>

class FixedWindow : public RateLimiter
{
public:
    FixedWindow(int limit, std::chrono::seconds windowSize);

    bool allow(const std::string& clientId) override;

private:
    struct Window
    {
        int count;
        std::chrono::steady_clock::time_point windowStart;
    };

    int limit_;
    std::chrono::seconds windowSize_;

    std::unordered_map<std::string, Window> windows_;

    std::mutex mutex_;
};