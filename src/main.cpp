#include <iostream>

#include <cmath>
#include <chrono>

#include "admin.hpp"

int main() {
    using namespace yLAB;
    auto start = std::chrono::high_resolution_clock::now(); 
    admin ad{std::cin};
    ad.print_intersected_triangles();
    auto end = std::chrono::high_resolution_clock::now(); 
    std::chrono::duration<double> dur = end - start;
    std::cout << "TIME: " << dur.count() << std::endl;
}
