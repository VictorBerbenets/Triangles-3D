#ifndef POINT_TYPE_
#define POINT_TYPE_

#include <iostream>
#include <cmath>
#include <array>

namespace yLAB {

struct point_t final {
    using coords = std::array<double, 3>;

    point_t(double x, double y, double z);
    point_t();
    ~point_t() = default;

    bool operator==(const point_t& other) const;
    
    point_t& operator+=(const point_t& other) noexcept;
    point_t& operator-=(const point_t& other) noexcept;
    point_t& operator*=(double coeff) noexcept;
    point_t& operator/=(double coeff);

    coords get_coords() const noexcept;
    bool is_valid() const noexcept;
//--------------------------------------------------------//
    double x_ = NAN, y_ = NAN, z_ = NAN;
};

point_t operator+(const point_t& lhs, const point_t& rhs) noexcept;
point_t operator-(const point_t& lhs, const point_t& rhs) noexcept;
point_t operator*(const point_t& lhs, double coeff) noexcept;
point_t operator*(double coeff, const point_t& rhs) noexcept;
point_t operator/(const point_t& lhs, double coeff);
point_t operator/(double coeff, const point_t& rhs);

std::ostream& operator<<(std::ostream& os, const point_t& pt);

} // <--- namespace yLAB

#endif
