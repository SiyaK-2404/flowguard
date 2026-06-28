#pragma once

#include <string>

class RateLimiter
{
public:
    virtual bool allow(const std::string& clientId) = 0;

    virtual ~RateLimiter() = default;
};