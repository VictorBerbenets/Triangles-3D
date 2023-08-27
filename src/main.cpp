#include "components.hpp"


int main() {
    using namespace yLAB;

    point_t p1(0, 0, 13);
    point_t p2(12, 0, 8);
    point_t p3(0, 12, 0);
    point_t p4(0, 0, -13);
    
    plane_t plane1(p1, p2, p3);
    plane_t plane2(p4, p2, p3);

    std::cout << (plane1 == plane2) << std::endl;
}
