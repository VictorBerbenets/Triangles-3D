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
 
    bool operator==(const line_t& other) const;
    bool is_parallel(const line_t& other) const;
    /*... intersect(const line_t& other) const {
        
    }*/
    vector_t get_coords() const noexcept;
    vector_t get_dirr_vec() const;
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
