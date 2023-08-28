#ifndef POINT_TYPE_
#define POINT_TYPE_

#include <cmath>

namespace yLAB {

struct point_t {
    point_t(double x, double y, double z);
    ~point_t() = default;

    bool operator==(const point_t& other) const;

    bool is_valid() const noexcept;
    void print() const;
//--------------------------------------------------------//
    double x_ = NAN, y_ = NAN, z_ = NAN;
};

}

#endif
