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
    vector_t coords = solve_equation(a_dirr, b_dirr, c_dirr, 0);
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

vector_t line_t::solve_equation(double a, double b, double c, double d) const {
    vector_t ret_val{};
    if (!is_equal(a, 0)) {
        ret_val[0] = -(b / a + c / a + d / a);
        ret_val[1] = ret_val[2] = 1;
    } else if (!is_equal(b, 0)) {
        ret_val[1] = -(a / b  + c / b + d / a);
        ret_val[0] = ret_val[2] = 1;
    } else {
        ret_val[2] = -(a / c + b / c + d / a);
        ret_val[0] = ret_val[1] = 1;
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
    bool result = is_parallel(other);
    if (!result) {
        return false;
    }

    if (is_equal(other.a_ / a_, other.d_ / d_)) {
        return true;
    }
    return false;
}
// if lines don't intersects or they equal then return NAN point_t
point_t line_t::get_intersec_point(const line_t& other) const {
    gener_type generator;
    init_generator(generator);
    
    point_t this_pt  = get_random_point();       // random this-line's point
    point_t other_pt = other.get_random_point(); // random other-line's point

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
    return solve_equation(a_, b_, c_, 0);
}

void line_t::init_generator(gener_type& generator) const {
    const u_int seed = static_cast<u_int>(std::time(nullptr));
    generator.seed(seed);
}

std::size_t line_t::random(gener_type& generator) const {
    std::uniform_int_distribution<std::size_t> distribution(MIN_VALUE, MAX_VALUE);
    return distribution(generator);
}

point_t line_t::get_random_point() const {
    // solving linear equation: ||a b c|-d|| - matrix form
    double x{}, y{}, z{};

    gener_type generator;
    init_generator(generator);

    if (!is_equal(a_, 0)) {
        y = random(generator);
        z = random(generator);
        x = -((b_ / a_) * y + (c_ / a_) * z + d_ / a_);
    } else if (!is_equal(b_, 0)) {
        x = random(generator);
        z = random(generator);
        y = -((a_ / b_) * x + (c_ / b_) * z + d_ / b_);      
    } else {
        x = random(generator);
        y = random(generator);
        z = -((a_ / c_) * x + (b_ / c_) * y + d_ / c_);
    }
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

