#include <iostream>
#include <stdexcept>
#include <random>
#include <ctime>
#include <cmath>

#include "line.hpp"
#include "utils.hpp"

namespace yLAB {

line_t::line_t(const point_t& pt1, const point_t& pt2) {
    if (pt1 == pt2) {
        throw std::invalid_argument{"points are equal, can't create 'line_t'"};
    }
    // finding cordinates of the dirrecting vector 
    double a_dirr = pt1.x_ - pt2.x_;
    double b_dirr = pt1.y_ - pt2.y_;
    double c_dirr = pt1.z_ - pt2.z_;
    //finding cordinates of the normal vector
    vector_t coords = solve_equation(a_dirr, b_dirr, c_dirr);
    a_ = coords[0];
    b_ = coords[1];
    c_ = coords[2];
    d_ = -(a_ * pt1.x_ + b_ * pt1.y_ + c_ * pt1.z_);
}

line_t::line_t(double a, double b, double c, double d):
    a_{a}, b_{b}, c_{c}, d_{d} {}

vector_t line_t::get_coords() const noexcept {
    return {a_, b_, c_};
}

vector_t line_t::solve_equation(double a, double b, double c, double d,
                                double rand_val1, double rand_val2) const {
    vector_t ret_val{};
    if (!is_equal(a, 0)) {
        ret_val[1] = rand_val1;
        ret_val[2] = rand_val2;
        ret_val[0] = -((b / a)*ret_val[1] + (c / a)*ret_val[2] + d / a);
    } else if (!is_equal(b, 0)) {
        ret_val[0] = rand_val1;
        ret_val[2] = rand_val2;
        ret_val[1] = -((a / b)*ret_val[0] + (c / b)*ret_val[2] + d / b);
    } else {
        ret_val[0] = rand_val1;
        ret_val[1] = rand_val2;
        ret_val[2] = -((a / c)*ret_val[0] + (b / c)*ret_val[1] + d / c);
    }
    return ret_val;
}

bool line_t::is_parallel(const line_t& other) const {
    vector_t vec_product = calc_vects_product(get_coords(), other.get_coords());
    if (is_null_vector(vec_product)) {
        return true;
    }
    return false; 
}

bool line_t::operator==(const line_t& other) const {
    auto [this_point1, this_point2] = get_points();
    // if other line contain two points of this line then they are equal
    if (other.contains(this_point1) && other.contains(this_point2)) {
        return true;
    }
    return false;
}
// if lines don't intersects or they equal then return NAN point_t
point_t line_t::get_intersec_point(const line_t& other) const {
    std::random_device rd;
    gener_type generator(rd());
    
    point_t this_pt  = get_point();       // random this-line's point
    point_t other_pt = other.get_point(); // random other-line's point
    
    //check whether lines lie in one plane}
    if (!is_complanar(get_coords(), other.get_coords(), get_vector(this_pt, other_pt))) {
        return {};
    }
    // can't get point if lines are parallel or equal
    if (is_parallel(other) || *this == other) {
        return {};
    }

    //vector_t coords = solve_equation(a_ - other.a_, b_ - other.b_, c_ - other.c_, d_ - other.d_);
    //return {coords[0], coords[1], coords[2]};
}

vector_t line_t::get_dirr_vec() const {
    return solve_equation(a_, b_, c_);
}

int line_t::random(gener_type& generator) const {
    std::uniform_int_distribution<int> distribution(MIN_VALUE, MAX_VALUE);
    return distribution(generator);
}

line_t::two_pts line_t::get_points() const {
    point_t pt1 = get_point();
    point_t pt2 = get_point();
    while(pt1 == pt2) {
        pt2 = get_point();
    }
    return {pt1, pt2};
}

point_t line_t::get_point() const {
    // solving linear equation: ||a b c|-d|| - matrix form
    std::random_device rd;
    gener_type generator(rd());
    auto [x, y, z] = solve_equation(a_, b_, c_, d_, random(generator), random(generator));
    return {x, y, z};
}

bool line_t::contains(const point_t& pt) const {
    double expr = a_ * pt.x_ + b_ * pt.y_ + c_ * pt.z_ + d_;
    return is_equal(expr, 0);
}

void line_t::print() const {
    std::cout << "a = " << a_ << " b = " << b_ << " c = " << c_
            << " d = " << d_ << std::endl;
}
//----------------------------------------------------------------------//

segment_t::segment_t(const point_t& pt1, const point_t& pt2):
        pt1_{pt1}, pt2_{pt2} {};

segment_t::segment_t(const point_t& pt):
        pt1_{pt}, pt2_{pt} {};

double segment_t::length() const {
    return std::sqrt( std::pow(pt1_.x_ - pt2_.x_, 2) + 
                      std::pow(pt1_.y_ - pt2_.y_, 2) + 
                      std::pow(pt1_.z_ - pt2_.z_, 2) );
}

bool segment_t::is_valid() const {
    return pt1_.is_valid() && pt2_.is_valid();
}

bool segment_t::is_intersect() const {

}

}

