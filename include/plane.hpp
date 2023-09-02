#ifndef PLANE_TYPE_
#define PLANE_TYPE_

#include "point.hpp"
#include "line.hpp"
#include "utils.hpp"

namespace yLAB {

struct plane_t final { // plane equation: Ax + By + Cz + D = 0
    plane_t(const point_t& pt1, const point_t& pt2, const point_t& pt3);
    plane_t(double A, double B, double C, double D);
    ~plane_t() = default;

    bool operator==(const plane_t& other) const;
    vector_t get_coords() const;
    bool is_parallel(const plane_t& other) const;
    bool contains(const point_t& pt) const;
    point_t get_plane_point() const;
    void print() const;
//------------------------------------------------------------------// 
    vector_t normal_coords_;
    double D_ = NAN;
};

} // <--- namespace yLAB
#endif
