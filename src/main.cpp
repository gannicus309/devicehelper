#include <iostream>
#include <exception>
#include "run.hpp"
#include <memory>

int main()
{
    try {
        devicehelper::run();
    } catch (std::exception& e) {
        return 1;
    }

    return 0;
}
