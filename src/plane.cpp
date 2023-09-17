#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
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
    vector_t vec = calc_vects_product(normal_coords_, other.normal_coords_);
    return calc_vects_product(normal_coords_, other.normal_coords_).is_null();
};

bool plane_t::contains(const point_t& pt) const {
    return is_zero(normal_coords_[0] * pt.x_ + normal_coords_[1] * pt.y_ + normal_coords_[2] * pt.z_ + D_);
}
// return plane point by solving linear equation
point_t plane_t::get_plane_point() const {
    // solving equation Ax + By + Cz + D = 0. Finding first not zero koeff.
    enum class Coords: int { X = 0, Y = 1, Z = 2 };
    
    using solve_pair = std::pair<double, Coords>;
    static constexpr double DEFAULT_COEFF = 1;

    std::array<solve_pair, 3> equation = {  solve_pair{normal_coords_[0], Coords::X},
                                            solve_pair{normal_coords_[1], Coords::Y},
                                            solve_pair{normal_coords_[2], Coords::Z} };
    auto not_zero_coeff = std::find_if(equation.begin(), equation.end(),
                                        [](auto&& val) {
                                            return !is_zero(val.first);
                                        } );
    auto [A, B, C] = normal_coords_.coords();
    switch(not_zero_coeff->second) {
        case Coords::X:
            return {-(D_ / A + B / A + C / A), DEFAULT_COEFF, DEFAULT_COEFF};
        case Coords::Y:
            return {DEFAULT_COEFF, -(D_ / B + A / B + C / B), DEFAULT_COEFF};
        case Coords::Z:
            return {DEFAULT_COEFF, DEFAULT_COEFF, -(D_ / C + A / C + B / C)};
        default:
            throw std::runtime_error{"All plane's coefficients = 0"};

    }
// we won't get here
    return {};
}

bool plane_t::operator==(const plane_t& other) const {
   return is_parallel(other) && other.contains(get_plane_point());
};

void plane_t::print() const {
    std::cout << normal_coords_[0] << "x + " << normal_coords_[1] << "y + " << normal_coords_[2] << "z + " << D_ << " = 0\n";
};

} // <--- namespace yLAB

