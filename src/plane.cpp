#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>

#include "plane.hpp"
#include "utils.hpp"
#include "point.hpp"
#include "line.hpp"

namespace yLAB {

plane_t::plane_t(const line_t& line, const point_t& pt) {

}

plane_t::plane_t(const point_t& pt1, const point_t& pt2, const point_t& pt3) {
    line_t ln(pt1, pt2);
    if (ln.contains(pt3)) {
        throw std::invalid_argument{"can't create a plane by noncolinear points"};
    }
    A_ = determ(pt2.y_ - pt1.y_, pt2.z_ - pt1.z_,
                pt3.y_ - pt1.y_, pt3.z_ - pt1.z_);
    B_ = -determ(pt2.x_ - pt1.x_, pt2.z_ - pt1.z_,
                pt3.x_ - pt1.x_, pt3.z_ - pt1.z_);
    C_ = determ(pt2.x_ - pt1.x_, pt2.y_ - pt1.y_,
                pt3.x_ - pt1.x_, pt3.y_ - pt1.y_);
    D_ = -(pt1.y_ * B_ + pt1.x_ * A_ + pt1.z_ * C_);
}

plane_t::plane_t(double A, double B, double C, double D):
            A_{A}, B_{B}, C_{C}, D_{D} {}

vector_t plane_t::get_coords() const {
    return {A_, B_, C_};
}

bool plane_t::is_parallel(const plane_t& other) const {
    vector_t vec_product = calc_vects_product(get_coords(), other.get_coords());
    if (is_null_vector(vec_product)) {
        return true;
    }
    return false; 
};

bool plane_t::contains(const point_t& pt) const {
    return are_equal(A_ * pt.x_ + B_ * pt.y_ + C_ * pt.z_ + D_, 0);
}

point_t plane_t::get_plane_point() const {
    double vec_module = std::sqrt( std::pow(A_, 2) + std::pow(B_, 2) + std::pow(C_, 2));
    if (are_equal(vec_module, 0)) {
        throw std::invalid_argument{"Modulus of the vector = 0\n"};
    }
    vector_t coords = get_coords();
    //  normalization of the vector 
    for (std::size_t id = 0; id < 3; ++id) {
        coords[id] /= vec_module;
    }
    return {D_ * coords[0], D_ * coords[1], D_ * coords[2]};

}

bool plane_t::operator==(const plane_t& other) const {
    bool result = is_parallel(other);
    if (!result) {
        return false;
    }

    if (other.contains(get_plane_point())) {
        return true;
    }
    return false;
};

void plane_t::print() const {
    std::cout << A_ << "x + " << B_ << "y + " << C_ << "z + " << D_ << " = 0\n";
};

}

