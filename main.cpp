#include <iostream>
#include <thread>

#include "slidingwindowcounter.hpp"

int main()
{
    SlidingWindowCounter limiter(3, std::chrono::seconds(5));

    for (int i = 1; i <= 5; i++)
    {
        std::cout << "Request " << i << ": "
                  << (limiter.allow("Alice") ? "Allowed" : "Rejected")
                  << '\n';
    }

    std::cout << "\nWaiting 3 seconds...\n\n";

    std::this_thread::sleep_for(std::chrono::seconds(3));

    for (int i = 6; i <= 8; i++)
    {
        std::cout << "Request " << i << ": "
                  << (limiter.allow("Alice") ? "Allowed" : "Rejected")
                  << '\n';
    }
}