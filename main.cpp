#include <iostream>
#include "tokenbucket.hpp"

int main()
{
    TokenBucket limiter;

    if (limiter.allow("Alice"))
    {
        std::cout << "Allowed\n";
    }

    return 0;
}