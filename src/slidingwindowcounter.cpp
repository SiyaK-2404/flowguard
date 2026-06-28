#include "slidingwindowcounter.hpp"

SlidingWindowCounter::SlidingWindowCounter(
    int limit,
    std::chrono::seconds windowSize)
    : limit_(limit),
      windowSize_(windowSize)
{
}

bool SlidingWindowCounter::allow(const std::string& clientId)
{
    std::lock_guard<std::mutex> lock(mutex_);

    auto now = std::chrono::steady_clock::now();

    auto& counter = counters_[clientId];

    // First request from this client
    if (counter.windowStart.time_since_epoch().count() == 0)
    {
        counter.windowStart = now;
    }

    auto elapsed = now - counter.windowStart;

    // Has at least one full window passed?
    if (elapsed >= windowSize_)
    {
        // If more than one window passed, previous window no longer matters
        if (elapsed >= windowSize_ * 2)
        {
            counter.previousCount = 0;
        }
        else
        {
            counter.previousCount = counter.currentCount;
        }

        counter.currentCount = 0;
        counter.windowStart = now;

        elapsed = std::chrono::steady_clock::duration::zero();
    }

    double elapsedSeconds =
        std::chrono::duration<double>(elapsed).count();

    double windowSeconds = windowSize_.count();

    double overlap = (windowSeconds - elapsedSeconds) / windowSeconds;

    double estimated =
        counter.currentCount +
        counter.previousCount * overlap;

    if (estimated < limit_)
    {
        counter.currentCount++;
        return true;
    }

    return false;
}