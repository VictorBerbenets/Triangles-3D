#include <iostream>
#include <stdexcept>
#include <array>
#include <utility>
#include <cmath>
#include <algorithm>

#include "line.hpp"
#include "utils.hpp"

namespace yLAB {

line_t::line_t(const point_t& pt1, const point_t& pt2) {
    if (pt1 == pt2) {
        throw std::invalid_argument{"points are equal, can't create 'line_t'"};
    }
    a1_ = pt1.x_ - pt2.x_;
    a2_ = pt1.y_ - pt2.y_;
    a3_ = pt1.z_ - pt2.z_;
    point_ = pt1;
}

line_t::line_t(double a1, double a2, double a3, const point_t& pt):
    a1_{a1}, a2_{a2}, a3_{a3}, point_{pt} {}

line_t::line_t(const vector_t& dirr_vec, const point_t& pt):
                a1_{dirr_vec[0]}, a2_{dirr_vec[1]}, a3_{dirr_vec[2]}, 
                point_{pt} {}

vector_t line_t::dir_coords() const noexcept {
    return {a1_, a2_, a3_};
}

bool line_t::is_parallel(const line_t& other) const {
    vector_t vec_product = calc_vects_product(dir_coords(), other.dir_coords());
    if (is_null_vector(vec_product)) {
        return true;
    }
    return false; 
}

bool line_t::operator==(const line_t& other) const {
    if (!is_parallel(other)) {
        return false;
    }
    // if other line contain a point of *this* line then they are equal
    if (other.contains(point_)) {
        return true;
    }
    return false;
}
// if lines don't intersects or they equal then return NAN point_t
point_t line_t::get_intersec_point(const line_t& other) const {
    point_t dir_comp = (point_ == other.point_) ? get_point() : point_;
    if (!are_complanar(dir_coords(), other.dir_coords(), get_vector(dir_comp, other.point_)) ||
            is_parallel(other)) { 
        return {};
    }
    std::array<double, 3> diffs { other.point_.x_ - point_.x_,
                                  other.point_.y_ - point_.y_,
                                  other.point_.z_ - point_.z_  };
                            
    using pair = std::pair<double, std::size_t>;
    std::array<pair, 3> dirs {pair{a1_, 0}, pair{a2_, 1}, pair{a3_, 2}};
    vector_t other_dirs = other.dir_coords();

    auto nzero_it = std::find_if(dirs.begin(), dirs.end(),
                                 [](auto&& val) { return !are_equal(val.first, 0); }
                                );
    double coeff{};
    for (std::size_t id = 1; id < 3; ++id) {
        std::size_t neigh_id = (nzero_it->second + id) % 3;
        double expr = other_dirs[neigh_id] * dirs[nzero_it->second].first - dirs[neigh_id].first * other_dirs[nzero_it->second];
        if (!are_equal(expr, 0)) {
            coeff = ( (dirs[neigh_id].first*diffs[nzero_it->second]) / dirs[nzero_it->second].first - diffs[neigh_id] ) * (dirs[nzero_it->second].first / expr);
        }
    }
    return { other.point_.x_ + other.a1_ * coeff,
             other.point_.y_ + other.a2_ * coeff,
             other.point_.z_ + other.a3_ * coeff };
}

point_t line_t::get_point(double coeff) const {
    return { point_.x_ + a1_ * coeff,
             point_.y_ + a2_ * coeff,
             point_.z_ + a3_ * coeff, };
}
bool line_t::contains(const point_t& pt) const {
    return is_null_vector(calc_vects_product(dir_coords(), get_vector(point_, pt)));
}

bool line_t::is_valid() const {
    return point_.is_valid() &&
           !std::isnan(a1_)  &&
           !std::isnan(a2_)  &&
           !std::isnan(a3_) ;
}

void line_t::print() const {
    std::cout << "a1 = " << a1_ << " a2 = " << a2_ << " a3 = " << a3_ << std::endl;
    std::cout << "point: ";
    point_.print();
    std::cout << std::endl;
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

bool segment_t::is_inside(const point_t& check_pt) const {
    if (is_degenerated()) {
        return pt1_ == check_pt;
    }
    segment_t segm_part1(check_pt, pt1_);
    segment_t segm_part2(check_pt, pt2_);
    return are_equal(length(), segm_part1.length() + segm_part2.length());
}

bool segment_t::is_intersect(const segment_t& other) const {
    // especial case
    if (is_degenerated()) {
        return other.is_inside(pt1_) ;
    }
    if (other.is_degenerated()) {
        return is_inside(other.pt1_) ;
    }

    line_t ln_this  {pt1_, pt2_};
    line_t ln_other {other.pt1_, other.pt2_};
    //if lie in one plane and parallel each other
    if( is_null_vector( calc_vects_product(ln_this.dir_coords(), ln_other.dir_coords()) ) ) {
        if (is_inside(other.pt1_) || is_inside(other.pt2_) ||
            other.is_inside(pt1_) || other.is_inside(pt2_) ) {
            return true;
        }
        return false;
    }
    // if not parallel
    point_t segm_pt = ln_this.get_intersec_point(ln_other);
    if (!segm_pt.is_valid()) {
        return false;
    }
    if (is_inside(segm_pt) && other.is_inside(segm_pt)) {
        return true;
    }
    return false;
}

bool segment_t::is_degenerated() const {
    return pt1_ == pt2_;
}

void segment_t::print() const {
    pt1_.print();
    pt2_.print();
}

}

