#include <iostream>
#include <cmath>

#include "point.hpp"
#include "utils.hpp"

namespace yLAB {

point_t::point_t(double x, double y, double z):
        x_{x}, y_{y}, z_{z} {};

bool point_t::is_valid() const noexcept {
    return is_equal(x_, NAN) && is_equal(y_, NAN) && is_equal(z_, NAN);
};

bool point_t::operator==(const point_t& other) const {
    return is_equal(x_, other.x_) &&
           is_equal(y_, other.y_) &&
           is_equal(z_, other.z_);
};

void point_t::print() const {
    std::cout << "x = " << x_ << " y = " << y_ << " z = " << z_ << std::endl;
}

}
