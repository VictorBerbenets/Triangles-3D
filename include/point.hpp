#ifndef POINT_TYPE_
#define POINT_TYPE_

#include <cmath>
#include <array>

#include "point.hpp"

namespace yLAB {

struct point_t {
    using coords = std::array<double, 3>;

    point_t(double x, double y, double z);
    point_t()  = default;
    ~point_t() = default;

    bool operator==(const point_t& other) const;
    
    point_t operator+(const point_t& other) const;
    point_t operator-(const point_t& other) const;
    point_t operator*(double coeff) const;
    point_t operator/(double coeff) const;
    
    coords get_coords() const;
    bool is_valid() const noexcept;
    void print() const;
//--------------------------------------------------------//
    double x_ = NAN, y_ = NAN, z_ = NAN;
};

}

#endif
