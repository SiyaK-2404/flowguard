#include "slidingwindowlog.hpp"

SlidingWindowLog::SlidingWindowLog(
    int limit,
    std::chrono::seconds windowSize)
    : limit_(limit),
      windowSize_(windowSize)
{
}

bool SlidingWindowLog::allow(const std::string& clientId)
{
    std::lock_guard<std::mutex> lock(mutex_);

    auto now = std::chrono::steady_clock::now();

    auto& log = logs_[clientId];

    // Remove expired timestamps
    while (!log.empty() &&
           now - log.front() >= windowSize_)
    {
        log.pop_front();
    }

    // Allow if under limit
    if (log.size() < limit_)
    {
        log.push_back(now);
        return true;
    }

    return false;
}