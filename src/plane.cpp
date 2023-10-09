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


bool plane_t::is_parallel(const plane_t& rhs) const {
    return calc_vects_product(normal_coords_, rhs.normal_coords_).is_null();
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

static plane_t::minor_pair find_not_zero_minor(const plane_t::plane_coeffs& coeffs1, const plane_t::plane_coeffs& coeffs2) {
    using size_type = plane_t::size_type;

    for (size_type index = 0; index < 3; ++index) {
        size_type next_index = (index + 1) % 3;
        double det = determ( coeffs1[index], coeffs1[next_index], coeffs2[index], coeffs2[next_index] );
        if (!is_zero(det)) {
            return {std::min(index, next_index), std::max(index, next_index)};
        }
    }
    return {NAN, NAN};
}

point_t plane_t::get_intersec_point(const plane_t& rhs) const {
    static constexpr int  ARBITRARY_VALUE = 1;

    plane_coeffs coeffs1 {normal_coords_[0], normal_coords_[1], normal_coords_[2], D_};
    plane_coeffs coeffs2 {rhs.normal_coords_[0], rhs.normal_coords_[1], rhs.normal_coords_[2], rhs.D_};
    auto [colum1, colum2] = find_not_zero_minor(coeffs1, coeffs2);
    if (std::isnan(colum1) || std::isnan(colum2)) {
        throw std::runtime_error{"couldn't find not zero minor\n"};
    }
    double not_zero_minor = determ( coeffs1[colum1], coeffs1[colum2],
                                    coeffs2[colum1], coeffs2[colum2] );
    size_type arbit_col = 3 - (colum1 + colum2); //colum1 + colum2 <=3
    double det_colum1 = determ(-coeffs1[3] - ARBITRARY_VALUE * coeffs1[arbit_col], coeffs1[colum2],
                        -coeffs2[3] - ARBITRARY_VALUE * coeffs2[arbit_col], coeffs2[colum2] );
    double det_colum2 = determ( coeffs1[colum1], -coeffs1[3] - ARBITRARY_VALUE * coeffs1[arbit_col],
                         coeffs2[colum1], -coeffs2[3] - ARBITRARY_VALUE * coeffs2[arbit_col] );
    using pair = std::pair<double, size_type>;
    std::array<pair, 3> determ_and_id = { pair{det_colum1, colum1},
                                          pair{det_colum2, colum2},
                                          pair{ARBITRARY_VALUE, arbit_col} };
    std::sort( determ_and_id.begin(), determ_and_id.end(),
                         [](auto&& val1, auto&& val2) { return val1.second < val2.second; }
             );
    return { 0 == arbit_col ? ARBITRARY_VALUE : determ_and_id[0].first / not_zero_minor,
             1 == arbit_col ? ARBITRARY_VALUE : determ_and_id[1].first / not_zero_minor,
             2 == arbit_col ? ARBITRARY_VALUE : determ_and_id[2].first / not_zero_minor };

}
line_t plane_t::get_intersec_line(const plane_t& rhs) const {
    vector_t dirr_vec = calc_vects_product(normal_coords_, rhs.normal_coords_);
    point_t intsec_pt = get_intersec_point(rhs);

    if (!intsec_pt.is_valid()) {
        throw std::runtime_error{"Couldn't get intersection point\n"};
    }
    return {dirr_vec, intsec_pt};

}

bool plane_t::operator==(const plane_t& rhs) const {
   return is_parallel(rhs) && rhs.contains(get_plane_point());
};

void plane_t::print() const {
    std::cout << normal_coords_[0] << "x + " << normal_coords_[1] << "y + " << normal_coords_[2] << "z + " << D_ << " = 0\n";
};

} // <--- namespace yLAB

