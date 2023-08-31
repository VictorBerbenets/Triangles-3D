#include <iostream>

#include "intersector.hpp"
#include <cmath>

int main() {
    using namespace yLAB;

    intersector triangles{std::cin};
    triangles.print_intersected_triangles();
}
