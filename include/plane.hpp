#ifndef PLANE_TYPE_
#define PLANE_TYPE_

#include <iostream>
#include <vector>

#include "utils.hpp"
#include "point.hpp"
#include "line.hpp"

namespace yLAB {

struct plane_t { // plane equation: Ax + By + Cz + D = 0
    plane_t(const line_t& line, const point_t& pt);
    plane_t(const point_t& pt1, const point_t& pt2, const point_t& pt3);
    ~plane_t() = default;

    bool operator==(const plane_t& other) const;
    std::pair<bool, double> is_parallel(const plane_t& other) const;
    void print() const;
//------------------------------------------------------------------// 
    double A_ = NAN, B_ = NAN, C_ = NAN, D_ = NAN;
};

};
#endif
