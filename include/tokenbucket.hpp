#pragma once

#include "ratelimiter.hpp"

#include <unordered_map>
#include <mutex>
#include <chrono>
#include <string>

class TokenBucket : public RateLimiter
{
public:
    TokenBucket(double capacity, double refillRate);

    bool allow(const std::string& clientId) override;

private:
    struct Bucket
    {
        double tokens;
        std::chrono::steady_clock::time_point lastRefill;
    };

    void refill(Bucket& bucket);

    double capacity_;
    double refillRate_;

    std::unordered_map<std::string, Bucket> buckets_;

    std::mutex mutex_;
};