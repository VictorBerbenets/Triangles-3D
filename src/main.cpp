#include "components.hpp"


int main() {
    using namespace yLAB;

    point_t p1(1, 2, 5);
    point_t p2(-1, 3, -6);

    line_t line(p1, p2);

    line.print();
}
