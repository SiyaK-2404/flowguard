#include "fixedwindow.hpp"

FixedWindow::FixedWindow(int limit,
                         std::chrono::seconds windowSize)
    : limit_(limit), windowSize_(windowSize)
{
}

bool FixedWindow::allow(const std::string& clientId)
{
    std::lock_guard<std::mutex> lock(mutex_);

    auto& window = windows_[clientId];
    auto now = std::chrono::steady_clock::now();

    if (window.windowStart.time_since_epoch().count() == 0)
    {
        window.count = 0;
        window.windowStart = now;
    }

  
    if (now - window.windowStart >= windowSize_)
    {
        window.count = 0;
        window.windowStart = now;
    }

    if (window.count < limit_)
    {
        window.count++;
        return true;
    }


    return false;
}