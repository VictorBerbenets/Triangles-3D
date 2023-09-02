#ifndef LINE_TYPE_
#define LINE_TYPE_

#include <cmath>
#include <utility>
#include <random>

#include "point.hpp"
#include "utils.hpp"

namespace yLAB {

struct line_t {       
    //using two_pts = std::pair<point_t, point_t>;
    static constexpr double DEFAULT_COEFF = 1;

    line_t(const point_t& pt1, const point_t& pt2);
    line_t(double a1, double a2, double a3, const point_t& pt);
    line_t(const vector_t& dirr_vec, const point_t& pt);
    ~line_t() = default;
 
    bool operator==(const line_t& other) const;
    bool contains(const point_t& pt) const;
    bool is_parallel(const line_t& other) const;
    bool is_valid() const;
    
    point_t get_intersec_point(const line_t& other) const;
    point_t get_point(double coeff = DEFAULT_COEFF) const;
    
    void print() const;
//------------------------------------------------------------------//
    vector_t dir_coords_; // coordinates of the direct vector 
    //double a1_ = NAN, a2_ = NAN, a3_ = NAN;
    point_t point_{};
};

struct segment_t {

    segment_t(const point_t& pt1, const point_t& pt2);
    segment_t(const point_t& pt);
    segment_t()  = default;
    ~segment_t() = default;
    
    double length() const;
    bool is_valid() const;
    bool is_intersect(const segment_t& other) const;
    bool is_inside(const point_t& pt) const;
    bool is_degenerated() const;

    void set_ends(const point_t& pt1, const point_t& pt2);
    void set_ends(const point_t& pt);
    void print() const;
//------------------------------------------------------------------//
    point_t pt1_{}, pt2_{};
};

}

#endif
