#ifndef LINE_TYPE_
#define LINE_TYPE_

#include <cmath>
#include <utility>

#include "point.hpp"
#include "vector.hpp"
#include "utils.hpp"

namespace yLAB {

struct line_t final {       
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
//------------------------------------------------------------------//
    vector_t dir_coords_{}; // coordinates of the direct vector 
    point_t point_{};
};

std::ostream& operator<<(std::ostream& os, const line_t& line);

struct segment_t final {

    segment_t(const point_t& pt1, const point_t& pt2);
    segment_t(const point_t& pt);
    segment_t()  = default;
    ~segment_t() = default;
    
    point_t center() const noexcept;
    
    double length() const;
    bool is_valid() const;
    bool is_intersect(const segment_t& other) const;
    bool is_inside(const point_t& pt) const;
    bool is_degenerated() const;
    
    void set_ends(const point_t& pt1, const point_t& pt2);
    void set_ends(const point_t& pt);
//------------------------------------------------------------------//
    point_t pt1_{}, pt2_{};
};

std::ostream& operator<<(std::ostream& os, const segment_t& segm);

} // <--- namespace yLAB

#endif
