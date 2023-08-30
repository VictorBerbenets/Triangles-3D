#ifndef LINE_TYPE_
#define LINE_TYPE_

#include <cmath>
#include <utility>
#include <random>

#include "point.hpp"
#include "utils.hpp"

namespace yLAB {

struct line_t {     // line view:   a_x + b_y + c_z + d_ = 0
    using two_pts = std::pair<point_t, point_t>;

    line_t(const point_t& pt1, const point_t& pt2);
    line_t(double a, double b, double c, double d);
    ~line_t() = default;
 
    bool operator==(const line_t& other) const;
    bool contains(const point_t& pt) const;
    bool is_parallel(const line_t& other) const;

    point_t get_intersec_point(const line_t& other) const;
    vector_t get_coords() const noexcept; 
    vector_t get_dirr_vec() const; 
    point_t get_point() const;  //random point on the line
    two_pts get_points() const; // two NOT EQUAL random points on the line

    void print() const;
private:
    static constexpr std::size_t MAX_VALUE = 100;
    static constexpr std::size_t MIN_VALUE = -100;

    using gener_type = std::mt19937;
    int random(gener_type& generator) const;
    // solving equation of such view: ax + by + cz + d = 0. rand_val1, rand_val2 - random values of two roots
    vector_t solve_equation(double a, double b, double c, double d = 0,
                            double rand_val1 = 1, double rand_val2 = 1) const;
public:
//------------------------------------------------------------------//
    double a_ = NAN, b_ = NAN, c_ = NAN, d_ = NAN;
};

struct segment_t {

    segment_t(const point_t& pt1, const point_t& pt2);
    segment_t(const point_t& pt);
    segment_t()  = default;
    ~segment_t() = default;
    
    bool is_valid() const;
    double length() const;
    bool is_intersect() const;
//------------------------------------------------------------------//
    point_t pt1_{}, pt2_{};
};

}

#endif
