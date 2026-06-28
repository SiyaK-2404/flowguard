#include "tokenbucket.hpp"
#include <algorithm>
TokenBucket::TokenBucket(double capacity, double refillRate)
    : capacity_(capacity), refillRate_(refillRate)
{
}

void TokenBucket::refill(Bucket& bucket)
{
    auto now = std::chrono::steady_clock::now();

    double elapsed =
        std::chrono::duration<double>(now - bucket.lastRefill).count();

    bucket.tokens = std::min(
        capacity_,
        bucket.tokens + elapsed * refillRate_);

    bucket.lastRefill = now;
}

bool TokenBucket::allow(const std::string& clientId)
{
    std::lock_guard<std::mutex> lock(mutex_);

    auto& bucket = buckets_[clientId];

    if (bucket.lastRefill.time_since_epoch().count() == 0)
    {
        bucket.tokens = capacity_;
        bucket.lastRefill = std::chrono::steady_clock::now();
    }

    refill(bucket);

    if (bucket.tokens >= 1.0)
    {
        bucket.tokens -= 1.0;
        return true;
    }

    return false;
}