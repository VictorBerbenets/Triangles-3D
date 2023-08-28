#ifndef LINE_TYPE_
#define LINE_TYPE_

#include <cmath>
#include <random>

#include "point.hpp"
#include "utils.hpp"

namespace yLAB {

struct line_t {     // line view:   a_x + b_y + c_z + d_ = 0
    line_t(const point_t& pt1, const point_t& pt2);
    line_t(double a, double b, double c, double d);
    ~line_t() = default;

    /*... intersect(const line_t& other) const {
        
    }*/
    coords_t get_dirr_vec() const;
    bool contains(const point_t& pt) const;
    void print() const;
    point_t get_random_point() const;
private:
    static constexpr std::size_t MAX_VALUE = 50;
    static constexpr std::size_t MIN_VALUE = -50;

    using gener_type = std::mt19937;

    void init_generator(gener_type& generator) const;
    std::size_t random(gener_type& generator) const;
public:
//------------------------------------------------------------------//
    double a_ = NAN, b_ = NAN, c_ = NAN, d_ = NAN;
};

struct segment {

    segment(const point_t& pt1, const point_t& pt2);
    ~segment() = default;

    double length() const;
    bool is_intersect() const;
//------------------------------------------------------------------//
    point_t point1_, point2_;
};

}

#endif
