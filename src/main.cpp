#include <iostream>

#include <cmath>

#include "admin.hpp"

int main() {
    using namespace yLAB;
    
    admin ad{std::cin};
    ad.print_intersected_triangles();
}
