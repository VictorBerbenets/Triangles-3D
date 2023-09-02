#include <iostream>
#include <cmath>

#include "point.hpp"
#include "utils.hpp"

namespace yLAB {

point_t::point_t(double x, double y, double z):
        x_{x}, y_{y}, z_{z} {};

point_t::vector point_t::get_coords() const {
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
    return {x_ / coeff, y_ / coeff, z_ / coeff};
}

void point_t::print() const {
    std::cout << "x = " << x_ << " y = " << y_ << " z = " << z_ << std::endl;
}

}
