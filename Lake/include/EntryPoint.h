#pragma once

#include <iostream>

extern const char* entryPointName;

int main() {
    std::cout << "Hello, World!" << std::endl;
    std::cout << "Entry point: " << entryPointName << std::endl;
    return 0;
}
