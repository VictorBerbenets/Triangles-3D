#ifndef TRIANGLE_TYPE_
#define TRIANGLE_TYPE_

#include <iostream>
#include <array>

#include "point.hpp"
#include "plane.hpp"

namespace yLAB {

class triangle_t {
    static constexpr std::size_t VERTICES_NUMBER = 3;
public:
    triangle_t(const point_t& pt1, const point_t& pt2, const point_t& pt3);
    triangle_t( double val1, double val2, double val3,
                double val4, double val5, double val6,
                double val7, double val8, double val9 );
    ~triangle_t() = default;

    plane_t get_plane() const;
private:
    std::array<point_t, VERTICES_NUMBER> vertices_;
};

};
#endif
