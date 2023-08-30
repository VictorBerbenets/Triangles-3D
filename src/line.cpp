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
        ret_val[1] = -(a / b  + c / b + d / b);
        ret_val[0] = ret_val[2] = 1;
    } else {
        ret_val[2] = -(a / c + b / c + d / c);
        ret_val[0] = ret_val[1] = 1;
    }
    return ret_val;
}

bool line_t::is_parallel(const line_t& other) const {
    vector_t vec_product = calc_vects_product(get_coords(), other.get_coords());
    /*std::cout << "vec product:\n";
    std::cout << "[0] = " << vec_product[0] << '\n';
    std::cout << "[1] = " << vec_product[1] << '\n';
    std::cout << "[2] = " << vec_product[2] << '\n';*/
    if (is_null_vector(vec_product)) {
        return true;
    }
    return false; 
}

bool line_t::operator==(const line_t& other) const {
    bool result = is_parallel(other);
    if (!result) {
        std::cout << "NOT PARALLEL\n\n";
        return false;
    }
    
    //point_t this_point1 = get_random_point();
    //point_t this_point2 = get_random_point();
    auto [this_point1, this_point2] = get_points();

    std::cout << "line equation: ";
    print();
    std::cout << "point1: ";
    this_point1.print();
    std::cout << "point2: ";
    this_point2.print();
    if (contains(this_point1) && contains(this_point2)) {
        std::cout << "generated points contain\n";
        return true;
    }
/*
    if (other.contains(this_point1) && other.contains(this_point2)) {
        return true;
    }*/
    /*if (is_equal(other.a_ / a_, other.d_ / d_)) {
        std::cout << "equal\n\n";
        return true;
    }*/
    std::cout << "not_equal\n\n";
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
    return solve_equation(a_, b_, c_, 0);
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
    double x{}, y{}, z{};
    
    std::random_device rd;
    gener_type generator(rd());

    std::cout << "print random numbers:\n";
    for (int i = 0; i < 10; ++i) {
        std::cout << random(generator) << ' ';
    }
    std::cout << '\n';
    if (!is_equal(a_, 0)) {
        y = random(generator);
        std::cout << "y gener = " << y << '\n';
        z = random(generator);
        std::cout << "z gener = " << z << '\n';
        x = -((b_ / a_) * y + (c_ / a_) * z + d_ / a_);
    } else if (!is_equal(b_, 0)) {
        x = random(generator);
        std::cout << "x gener = " << x << '\n';
        z = random(generator);
        std::cout << "z gener = " << z << '\n';
        y = -((a_ / b_) * x + (c_ / b_) * z + d_ / b_);      
    } else {
        x = random(generator);
        std::cout << "x gener = " << x << '\n';
        y = random(generator);
        std::cout << "y gener = " << y << '\n';
        z = -((a_ / c_) * x + (b_ / c_) * y + d_ / c_);
    }
    return {x, y, z};
}

bool line_t::contains(const point_t& pt) const {
    double expr = a_ * pt.x_ + b_ * pt.y_ + c_ * pt.z_ + d_;
    std::cout << "point in line: " << expr << '\n';
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

