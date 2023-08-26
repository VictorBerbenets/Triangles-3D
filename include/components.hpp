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

    /*... intersect(const line_t& other) const {
        
    }*/
    
    bool contains(const point_t& pt) {
        double expr = a_ * pt.x_ + b_ * pt.y_ + c_ * pt.z_ + d_;
        return is_equal(expr, 0);
    }

    void print() {
        std::cout << "a = " << a_ << " b = " << b_ << " c = " << c_
                << " d = " << d_ << std::endl;
    };
//------------------------------------------------------------------// 
    double a_ = NAN, b_ = NAN, c_ = NAN, d_ = NAN;
};

struct plane_t {
    plane_t(const line_t& line, const point_t& pt) {
    
    }

    plane_t(const point_t& pt1, const point_t& pt2, const point_t& pt3) {
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
    ~plane_t() = default;
    
    double determ(double a, double b, double c, double d) { //|a b|
        return a * d - c * b;                               //|c d| = a * d - c * b
    }
    void print() {
        std::cout << A_ << "x + " << B_ << "y + " << C_ << "z + " << D_ << " = 0\n";
    };
//------------------------------------------------------------------// 
    double A_ = NAN, B_ = NAN, C_ = NAN, D_ = NAN;
};

} // <-- namespace yLAB

