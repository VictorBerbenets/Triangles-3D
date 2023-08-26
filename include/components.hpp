#include <iostream>
#include <cmath>

namespace yLAB {

bool is_equal(double val1, double val2) {
    return std::fabs(val1 - val2) < std::numeric_limits<double>::epsilon();
}

struct point_t {
    point_t(double x, double y, double z):
            x_{x}, y_{y}, z_{z} {};
    ~point_t() = default;

    bool is_valid() const noexcept {
        return x_ != NAN && y_ != NAN && z_ != NAN;
    };

    bool operator==(const point_t& other) const noexcept {
        return is_equal(x_, other.x_) &&
               is_equal(y_, other.y_) &&
               is_equal(z_, other.z_);
    };

    void print() {
        std::cout << "x = " << x_ << " y = " << y_ << " z = " << z_ << std::endl;
    }
//--------------------------------------------------------//    
    double x_ = NAN, y_ = NAN, z_ = NAN;
};

struct line_t {     // line view:   a_x + b_y + c_z + d_ = 0
    line_t(const point_t& pt1, const point_t& pt2) {
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
    };

    ~line_t() = default;

    void print() {
        std::cout << "a = " << a_ << " b = " << b_ << " c = " << c_
                << " d = " << d_ << std::endl;
    }
//------------------------------------------------------------------// 
    double a_ = NAN, b_ = NAN, c_ = NAN, d_ = NAN;
};



} // <-- namespace yLAB

