#include <iostream>
#include <cmath>
#include <stdexcept>

#include "vector.hpp"
#include "point.hpp"
#include "utils.hpp"

namespace yLAB {

vector_t::vector_t(value_type coord1, value_type coord2, value_type coord3):
                coords_ {coord1, coord2, coord3} {}

vector_t::vector_t(const point_t& pt1, const point_t& pt2):
                coords_ {pt2.x_ - pt1.x_, pt2.y_ - pt1.y_, pt2.z_ - pt1.z_} {}

vector_t::vector_t(value_type coord):
                coords_{coord, coord, coord} {}

vector_t::vector_t(): vector_t(NAN) {}

bool vector_t::operator==(const vector_t& rhs) const noexcept {
    return are_equal(coords_[0], rhs[0]) &&
           are_equal(coords_[1], rhs[1]) &&
           are_equal(coords_[2], rhs[2]) ;
}

vector_t operator+(const vector_t& lhs, const vector_t& rhs) noexcept {
    return {lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2]};
}

vector_t operator-(const vector_t& lhs, const vector_t& rhs) noexcept {
    return {lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2]};
}

vector_t& vector_t::operator*=(value_type coeff) noexcept {
    for (auto& val : coords_) {
        val *= coeff;
    }
    return *this;
}

vector_t& vector_t::operator/=(value_type coeff) {
    if (is_zero(coeff)) {
        throw std::invalid_argument{"Attempt to divide by 0 in vector's operator/=\n"};
    }
    for (auto& val : coords_) {
        val /= coeff;
    }
    return *this;
}

vector_t operator*(const vector_t& vec, vector_t::value_type coeff) noexcept {
    vector_t copy = vec;
    copy *= coeff;
    return copy;
}

vector_t operator*(vector_t::value_type coeff, const vector_t& vec) noexcept {
    vector_t copy = vec;
    copy *= coeff;
    return copy;
}

vector_t vector_t::operator/(value_type coeff) const {
    if (is_zero(coeff)) {
        throw std::invalid_argument{"Attempt to divide by 0 in vector's operator/\n"};
    }
    return {coords_[0] / coeff, coords_[1] / coeff, coords_[2] / coeff};
}

vector_t::value_type& vector_t::operator[](size_type index) {
    return coords_[index];
}

vector_t::value_type vector_t::operator[](size_type index) const {
    return coords_[index];
}

vector_t::value_type& vector_t::at(size_type index) {
    if (index > 2) {
        throw std::out_of_range{"index out of range in vector's operator()\n"};
    }
    return coords_[index];
}

vector_t::value_type vector_t::at(size_type index) const {
    if (index > 2) {
        throw std::out_of_range{"index out of range in vector's operator()\n"};
    }
    return coords_[index];
}

vector_t::value_type vector_t::get_module() const {
    return std::sqrt( coords_[0] * coords_[0] + coords_[1] * coords_[1] + coords_[2] * coords_[2] );
}

vector_t::array vector_t::coords() const noexcept {
    return coords_;
}

bool vector_t::is_null() const {
    return is_zero(coords_[0]) &&
           is_zero(coords_[1]) &&
           is_zero(coords_[2]) ;
}

bool vector_t::is_valid() const {
    return !std::isnan(coords_[0]) &&
           !std::isnan(coords_[1]) &&
           !std::isnan(coords_[2]) ;
}

vector_t::size_type vector_t::size() const noexcept {
    return coords_.size();
}

std::ostream& operator<<(std::ostream& os, const vector_t& rhs) {
    return os << rhs[0] << ' ' << rhs[1] << ' ' << rhs[2]; 
}

} // <--- namespace yLAB

