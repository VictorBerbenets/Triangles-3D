#include "plane.hpp"

namespace yLAB {

plane_t::plane_t(const line_t& line, const point_t& pt) {

}

plane_t::plane_t(const point_t& pt1, const point_t& pt2, const point_t& pt3) {
    line_t ln(pt1, pt2);
    if (ln.contains(pt3)) {
        throw std::invalid_argument{"can't create a plane by noncolinear points"};
    }
    A_ = determ(pt2.y_ - pt1.y_, pt2.z_ - pt1.z_,
                pt3.y_ - pt1.y_, pt3.z_ - pt1.z_);
    B_ = -determ(pt2.x_ - pt1.x_, pt2.z_ - pt1.z_,
                pt3.x_ - pt1.x_, pt3.z_ - pt1.z_);
    C_ = determ(pt2.x_ - pt1.x_, pt2.y_ - pt1.y_,
                pt3.x_ - pt1.x_, pt3.y_ - pt1.y_);
    D_ = -(pt1.y_ * B_ + pt1.x_ * A_ + pt1.z_ * C_);
}

std::pair<bool, double> plane_t::is_parallel(const plane_t& other) const {
    std::vector<double> comp_coeffs{};
    if (is_equal(A_, 0)) {
        if (!is_equal(other.A_, 0)) {
            return {false, 0};
        }
    } else {
        comp_coeffs.push_back(other.A_ / A_);
    }
    if (is_equal(B_, 0)) {
        if (!is_equal(other.B_, 0)) {
            return {false, 0};
        }
    } else {
        comp_coeffs.push_back(other.B_ / B_);
    }
    if (is_equal(C_, 0)) {
        if (!is_equal(other.C_, 0)) {
            return {false, 0};
        }
    } else {
        comp_coeffs.push_back(other.C_ / C_);
    }
    //if all coeffs are equal -> planes are parallel
    bool is_par = std::all_of(comp_coeffs.begin(), comp_coeffs.end(),
            [&comp_coeffs](auto val) {
                return is_equal(val, comp_coeffs.front());
            }
            );
    return {is_par, comp_coeffs.front()}; // pair.first - is parallel or not
                                          // pair.second - koeff. of  proporcionality of 
                                          // the plane's normal vectors(useful when pair.first = true)
};

bool plane_t::operator==(const plane_t& other) const {
    auto result = is_parallel(other);
    if (!result.first) {
        return false;
    }
    if (is_equal(D_, 0)) {
        if (!is_equal(other.D_, 0)) {
            return false;
        }
    } else if (!is_equal(other.D_ / D_, result.second)) {
        return false;
    }
    return true;
};

void plane_t::print() const {
    std::cout << A_ << "x + " << B_ << "y + " << C_ << "z + " << D_ << " = 0\n";
};

};

