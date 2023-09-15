#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>

#include "vector.hpp"
#include "plane.hpp"
#include "utils.hpp"
#include "point.hpp"
#include "line.hpp"

namespace yLAB {

plane_t::plane_t(const point_t& pt1, const point_t& pt2, const point_t& pt3) {
    line_t ln(pt1, pt2);
    if (ln.contains(pt3)) {
        throw std::invalid_argument{"can't create a plane by noncolinear points"};
    }
    normal_coords_ = calc_vects_product(vector_t{pt1, pt2}, vector_t{pt1, pt3});
    D_ = -(pt1.y_ * normal_coords_[1] + pt1.x_ * normal_coords_[0] + pt1.z_ * normal_coords_[2]);
}

plane_t::plane_t(double A, double B, double C, double D):
            normal_coords_ {A, B, C}, D_{D} {}


bool plane_t::is_parallel(const plane_t& other) const {
    return calc_vects_product(normal_coords_, other.normal_coords_).is_null();
};

bool plane_t::contains(const point_t& pt) const {
    return is_zero(normal_coords_[0] * pt.x_ + normal_coords_[1] * pt.y_ + normal_coords_[2] * pt.z_ + D_);
}
// return plane point by knowing D and normal vector
point_t plane_t::get_plane_point() const {
    double vec_module = normal_coords_.get_module(); 
    if (is_zero(vec_module)) {
        throw std::invalid_argument{"Modulus of the vector = 0\n"};
    }
    double mul_component = D_ / vec_module;
    return { mul_component * normal_coords_[0],
             mul_component * normal_coords_[1],
             mul_component * normal_coords_[2] };
}

bool plane_t::operator==(const plane_t& other) const {
    return is_parallel(other) && other.contains(get_plane_point());
};

void plane_t::print() const {
    std::cout << normal_coords_[0] << "x + " << normal_coords_[1] << "y + " << normal_coords_[2] << "z + " << D_ << " = 0\n";
};

} // <--- namespace yLAB

