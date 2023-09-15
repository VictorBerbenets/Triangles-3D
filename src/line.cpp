#include <iostream>
#include <stdexcept>
#include <array>
#include <utility>
#include <cmath>
#include <algorithm>

#include "line.hpp"
#include "utils.hpp"

namespace yLAB {

line_t::line_t(const point_t& pt1, const point_t& pt2):
            dir_coords_{pt1, pt2},
            point_{pt1}  {
    if (pt1 == pt2) {
        throw std::invalid_argument{"points are equal, can't create 'line_t'"};
    }
}

line_t::line_t(double a1, double a2, double a3, const point_t& pt):
             dir_coords_{a1, a2, a3}, point_{pt} {}

line_t::line_t(const vector_t& dirr_vec, const point_t& pt):
             dir_coords_{dirr_vec},
             point_{pt} {}

bool line_t::is_parallel(const line_t& other) const {
    vector_t vec_product = calc_vects_product(dir_coords_, other.dir_coords_);
    return vec_product.is_null();
}

bool line_t::operator==(const line_t& other) const {
    // if other line contain a point of *this* line then they are equal
    return is_parallel(other) && other.contains(point_);
}
// if lines don't intersects or they equal then return NAN point_t
point_t line_t::get_intersec_point(const line_t& other) const {
    std::array<double, 3> diffs { other.point_.x_ - point_.x_,
                                  other.point_.y_ - point_.y_,
                                  other.point_.z_ - point_.z_  };
                            
    using pair = std::pair<double, std::size_t>;
    std::array<pair, 3> dirs { pair{dir_coords_[0], 0}, pair{dir_coords_[1], 1}, pair{dir_coords_[2], 2} };

    auto nzero_it = std::find_if(dirs.begin(), dirs.end(),
                                 [](auto&& val) { return !is_zero(val.first); }
                                );
    double coeff{};
    // solving system of linear equations to find common point
    for (std::size_t id = 1; id < 3; ++id) {
        std::size_t neigh_id = (nzero_it->second + id) % 3;
        auto val = dirs[nzero_it->second].first;
        double expr = other.dir_coords_[neigh_id] * val - dirs[neigh_id].first * other.dir_coords_[nzero_it->second];
        if (!is_zero(expr)) {
            coeff = ( (dirs[neigh_id].first*diffs[nzero_it->second]) / val - diffs[neigh_id] ) * (val / expr);
        }
    }
    return other.get_point(coeff);
}

point_t line_t::get_point(double coeff) const {
    return { point_.x_ + dir_coords_[0] * coeff,
             point_.y_ + dir_coords_[1] * coeff,
             point_.z_ + dir_coords_[2] * coeff, };
}
bool line_t::contains(const point_t& pt) const {
    return calc_vects_product(dir_coords_, {point_, pt}).is_null();
}

bool line_t::is_valid() const {
    return point_.is_valid() &&
           dir_coords_.is_valid();
}

std::ostream& operator<<(std::ostream& os, const line_t& line) {
    return os << line.dir_coords_ << std::endl << line.point_ << std::endl;
}
//----------------------------------------------------------------------//

segment_t::segment_t(const point_t& pt1, const point_t& pt2):
        pt1_{pt1}, pt2_{pt2} {};

segment_t::segment_t(const point_t& pt):
        pt1_{pt}, pt2_{pt} {};

point_t segment_t::center() const noexcept {
    return { (pt1_.x_ + pt2_.x_) / 2, (pt1_.y_ + pt2_.y_) / 2, (pt1_.z_ + pt2_.z_) / 2 };
}

double segment_t::length() const {
    auto sq_len1 = pt2_.x_ - pt1_.x_;
    auto sq_len2 = pt2_.y_ - pt1_.y_;
    auto sq_len3 = pt2_.z_ - pt1_.z_;
    return std::sqrt(sq_len1 * sq_len1 + sq_len2 * sq_len2 + sq_len3 * sq_len3);
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
    // corner case
    if (is_degenerated()) {
        return other.is_inside(pt1_) ;
    }
    if (other.is_degenerated()) {
        return is_inside(other.pt1_) ;
    }

    line_t ln_this  {pt1_, pt2_};
    line_t ln_other {other.pt1_, other.pt2_};
    //if lie in one plane and parallel each other
    if( calc_vects_product(ln_this.dir_coords_, ln_other.dir_coords_).is_null() ) {
        if (is_inside(other.pt1_) || is_inside(other.pt2_) ||
            other.is_inside(pt1_) || other.is_inside(pt2_) ) {
            return true;
        }
        return false;
    }
    // if not parallel
    point_t segm_pt = ln_this.get_intersec_point(ln_other);
    return segm_pt.is_valid() && is_inside(segm_pt) && other.is_inside(segm_pt);
}

bool segment_t::is_degenerated() const {
    return pt1_ == pt2_;
}

void segment_t::set_ends(const point_t& pt1, const point_t& pt2) {
    pt1_ = pt1;
    pt2_ = pt2;
}

void segment_t::set_ends(const point_t& pt) {
    pt1_ = pt2_ = pt;
}

std::ostream& operator<<(std::ostream& os, const segment_t& segm) {
    return os << segm.pt1_ << std::endl << segm.pt2_;
}

}// namespace yLAB

