#include <iostream>
#include <cmath>

namespace yLAB {

bool is_equal(double val1, double val2) {
    return std::fabs(val1 - val2) < std::numeric_limits<double>::epsilon();
}

struct point_t {
    point_t(double x, double y, double z):
            x_{x}, y_{y}, z_{z} {};

    bool is_valid() const noexcept {
        return x_ != NAN && y_ != NAN && z_ != NAN;
    };

    bool equal(const point_t& other) const noexcept {
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

class line_t {     // line view:   ax + by + cz + d = 0
public:
    line_t(const point_t& pt1, const point_t& pt2) {
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
        } else if (!is_equal(c_dirr, 0)) {
            c_ = -(a_dirr/c_dirr + b_dirr/c_dirr);
            a_ = b_ = 1;
        } else {
            std::cout << "point's are equal\n";
            valid = false;
        }
        if (valid) {
            d_ = -(a_ * pt1.x_ + b_ * pt1.y_ + c_ * pt1.z_);
        }
    };

    void print() {
        std::cout << "a = " << a_ << " b = " << b_ << " c = " << c_
                << " d = " << d_ << std::endl;
    }
private: 
    double a_ = NAN, b_ = NAN, 
           c_ = NAN, d_ = NAN;
    bool valid = true;
};

} // <-- namespace yLAB

