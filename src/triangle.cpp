#include <iostream>

#include "triangle.hpp"
//#include "intersector.hpp"

namespace yLAB {

triangle_t::triangle_t(const point_t& pt1, const point_t& pt2, const point_t& pt3):
    vertices_{pt1, pt2, pt3} {};

triangle_t::triangle_t( double val1, double val2, double val3,
                        double val4, double val5, double val6,
                        double val7, double val8, double val9 ):
    vertices_{ point_t{val1, val2, val3},
               point_t{val4, val5, val6},
               point_t{val7, val8, val9} } {};

plane_t triangle_t::get_plane() const {
    return {vertices_[0], vertices_[1], vertices_[2]};
}

const point_t& triangle_t::operator[](std::size_t index) const {
    return vertices_[index];
}

point_t& triangle_t::operator[](std::size_t index) {
    return vertices_[index];
}

std::ostream& operator<<(std::ostream& os, const triangle_t& tria) {
    os << tria[0] << std::endl;
    os << tria[1] << std::endl;
    return os << tria[2];
}

} // <--- namespace yLAB
