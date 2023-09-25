#ifndef TRIANGLE_TYPE_
#define TRIANGLE_TYPE_

#include <iostream>
#include <array>

#include "point.hpp"
#include "plane.hpp"

namespace yLAB {

class triangle_t final {
    using size_type = std::size_t;

    bool check_segments_intersection(const triangle_t& rhs) const;
    bool check_inside_point(const triangle_t& rhs) const;
    bool same_intersection(const triangle_t& rhs) const;
public:
    using pts_vector = std::array<point_t, 3>;

    triangle_t(const point_t& pt1, const point_t& pt2, const point_t& pt3);
    triangle_t( double val1, double val2, double val3,
                double val4, double val5, double val6,
                double val7, double val8, double val9 );
    ~triangle_t() = default;
    
    bool is_intersecting(const triangle_t& rhs) const;

    const point_t& operator[](std::size_t index) const;
    point_t& operator[](std::size_t index);
    
    plane_t get_plane() const;
private:
    pts_vector vertices_;
};

std::ostream& operator<<(std::ostream& os, const triangle_t& tria);

} // <--- namespace yLAB

#endif
