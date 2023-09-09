#include <iostream>
#include <cmath>

#include "point.hpp"
#include "utils.hpp"

namespace yLAB {

point_t::point_t(double x, double y, double z):
        x_{x}, y_{y}, z_{z} {};

point_t::point_t():
        x_{NAN}, y_{NAN}, z_{NAN} {};

point_t::coords point_t::get_coords() const {
    return {x_, y_, z_};
}

bool point_t::is_valid() const noexcept {
    return !std::isnan(x_) && !std::isnan(y_) && !std::isnan(z_);
};

bool point_t::operator==(const point_t& other) const {
    return are_equal(x_, other.x_) &&
           are_equal(y_, other.y_) &&
           are_equal(z_, other.z_);
};

point_t point_t::operator+(const point_t& other) const {
    return {x_ + other.x_, y_ + other.y_, z_ + other.z_};
}

point_t point_t::operator-(const point_t& other) const {
    return {x_ - other.x_, y_ - other.y_, z_ - other.z_};
}

point_t point_t::operator*(double coeff) const {
    return {x_ * coeff, y_ * coeff, z_ * coeff};
}

point_t point_t::operator/(double coeff) const {
    if (is_zero(coeff)) {
        throw std::invalid_argument{"Attempt to divide by zero in point's operator/\n"};
    }
    return {x_ / coeff, y_ / coeff, z_ / coeff};
}

std::ostream& operator<<(std::ostream& os, const point_t& pt) {
    return os << pt.x_ << ' ' << pt.y_ << ' ' << pt.z_;
}

} // <--- namespace yLAB
