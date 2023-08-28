#include <iostream>
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
    if (!is_equal(a_dirr, 0)) {
        a_ = -(b_dirr/a_dirr + c_dirr/a_dirr);
        b_ = c_ = 1;
    } else if (!is_equal(b_dirr, 0)) {
        b_ = -(a_dirr/b_dirr + c_dirr/b_dirr);
        a_ = c_ = 1;
    } else {
        c_ = -(a_dirr/c_dirr + b_dirr/c_dirr);
        a_ = b_ = 1;
    }
    d_ = -(a_ * pt1.x_ + b_ * pt1.y_ + c_ * pt1.z_);
}

line_t::line_t(double a, double b, double c, double d):
    a_{a}, b_{b}, c_{c}, d_{d} {};
    
bool line_t::contains(const point_t& pt) const {
    double expr = a_ * pt.x_ + b_ * pt.y_ + c_ * pt.z_ + d_;
    return is_equal(expr, 0);
}

void line_t::print() const {
    std::cout << "a = " << a_ << " b = " << b_ << " c = " << c_
            << " d = " << d_ << std::endl;
}
//----------------------------------------------------------------------//

segment::segment(const point_t& pt1, const point_t& pt2):
        point1_{pt1}, point2_{pt2} {};

double segment::length() const {
    return std::sqrt( std::pow(point1_.x_ - point2_.x_, 2) + 
                      std::pow(point1_.y_ - point2_.y_, 2) + 
                      std::pow(point1_.z_ - point2_.z_, 2) );
}

bool segment::is_intersect() const {

}

}

