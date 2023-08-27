#include <iostream>
#include <cmath>
#include <vector>
#include <array>
#include <unordered_set>

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

    bool operator==(const point_t& other) const {
        return is_equal(x_, other.x_) &&
               is_equal(y_, other.y_) &&
               is_equal(z_, other.z_);
    };

    void print() const {
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

    line_t(double a, double b, double c, double d):
        a_{a}, b_{b}, c_{c}, d_{d} {};
    
    ~line_t() = default;

    /*... intersect(const line_t& other) const {
        
    }*/
    
    bool contains(const point_t& pt) const {
        double expr = a_ * pt.x_ + b_ * pt.y_ + c_ * pt.z_ + d_;
        return is_equal(expr, 0);
    }

    void print() const {
        std::cout << "a = " << a_ << " b = " << b_ << " c = " << c_
                << " d = " << d_ << std::endl;
    };
//------------------------------------------------------------------// 
    double a_ = NAN, b_ = NAN, c_ = NAN, d_ = NAN;
};

struct segment {
    
    segment(const point_t& pt1, const point_t& pt2):
        point1_{pt1}, point2_{pt2} {};
    ~segment() = default;

    double length() const {
        return std::sqrt( std::pow(point1_.x_ - point2_.x_, 2) + 
                          std::pow(point1_.y_ - point2_.y_, 2) + 
                          std::pow(point1_.z_ - point2_.z_, 2) );
    };

    bool is_intersect() const {
    
    }
//------------------------------------------------------------------// 
    point_t point1_, point2_;
};

inline double determ(double a, double b, double c, double d) { //|a b|
    return a * d - c * b;                               //|c d| = a * d - c * b
};

struct plane_t { // plane equation: Ax + By + Cz + D = 0
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

    std::pair<bool, double> is_parallel(const plane_t& other) const {
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

    bool operator==(const plane_t& other) const {
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

    void print() const {
        std::cout << A_ << "x + " << B_ << "y + " << C_ << "z + " << D_ << " = 0\n";
    };
//------------------------------------------------------------------// 
    double A_ = NAN, B_ = NAN, C_ = NAN, D_ = NAN;
};

class triangle_t {
    static constexpr std::size_t VERTICES_NUMBER = 3;
public:
    triangle_t(const point_t& pt1, const point_t& pt2, const point_t& pt3):
        vertices_{pt1, pt2, pt3} {};
    triangle_t( double val1, double val2, double val3,
                double val4, double val5, double val6,
                double val7, double val8, double val9 ):
        vertices_{ point_t{val1, val2, val3},
                   point_t{val4, val5, val6},
                   point_t{val7, val8, val9} } {};
    

    ~triangle_t() = default;

    plane_t get_plane() const {
        return plane_t{vertices_[0], vertices_[1], vertices_[2]};
    }

private:
    std::array<point_t, VERTICES_NUMBER> vertices_;
};

class intersector {
    using size_type   = std::size_t;
    
    static constexpr size_type VARIABLES_NUMBER = 8;
    static constexpr size_type SET_POINTS_SIZE  = 9;

    enum class Coeffs: char {A = 0, B = 1, C = 2, D = 3}; // Ax + By + Cz + D = 0 - plane's equation

    using data_val    = std::pair<triangle_t, size_type>; // saving triangle and his order number
    using printPair   = std::pair<size_type, size_type>;
    using solvePair   = std::pair<double, Coeffs>;
    using solveData   = std::array<solvePair, VARIABLES_NUMBER>;  // each plane contains A, B, C, D --> two planes contain 8 arguments

    point_t solve_linear_equations(const solveData& data) const;
    line_t get_intersection_line(const plane_t& plane1, const plane_t plane2) const;
    void swap_first_column(const solveData& data) const;
    void swap_second_column(const solveData& data) const;
    void swap_columns(const solvePair& up_column1, const solvePair& down_column1,
                      const solvePair& up_column2, const solvePair& down_column2) const; 
public:
    intersector(std::istream& is);
    ~intersector() = default;

    void print_intersected_triangles() const;
    printPair different_intersection(const triangle_t& tria1, const plane_t& plane1,
                                     const triangle_t& tria2, const plane_t& plane2) const;
private:
    std::vector<data_val> data_;
    std::istream& stream_;
};

intersector::intersector(std::istream& is):
    stream_{is} {
    size_type data_size{};
    stream_ >> data_size;
    if (!stream_.good()) {
        throw std::runtime_error{"data size reading error\n"};
    }
    data_.reserve(data_size);

    std::vector<double> tmp_points{};
    tmp_points.reserve(SET_POINTS_SIZE);
    //  processing of input data 
    for (size_type count = 1; count <= data_size; ++count) {
        double tmp_value{};
        for (size_type points_number = 0; points_number < SET_POINTS_SIZE; ++points_number) {
            stream_ >> tmp_value;
            if (!stream_.good()) {
                throw std::runtime_error{"data reading error\n"};
            }
            tmp_points.push_back(tmp_value);
        }
        data_.emplace_back(triangle_t { {tmp_points[0], tmp_points[1], tmp_points[2]},
                                        {tmp_points[3], tmp_points[4], tmp_points[5]},
                                        {tmp_points[6], tmp_points[7], tmp_points[8]} }, count);
        tmp_points.clear();
    };
};

void intersector::print_intersected_triangles() const {
    std::unordered_set<size_type> intersec_triangles{};
    for (auto iter1 = data_.begin(); iter1 != data_.end(); ++iter1) {
        auto comp_plane = iter1->first.get_plane();
        for (auto iter2 = (iter1 + 1); iter2 != data_.end(); ++iter2) {
            auto tmp_plane = iter2->first.get_plane();
            if (comp_plane == tmp_plane) { // both triangles lies in one plane

            } else if (comp_plane.is_parallel(tmp_plane).first) {
                // these pair of triangles can't intersect each other
                continue;
            } else { // both triangles lies in different planes
               auto pair = different_intersection(iter1->first, comp_plane,
                                                  iter2->first, tmp_plane); 
            }
        }
    }
};

intersector::printPair intersector::different_intersection(const triangle_t& tria1, const plane_t& plane1,
                                     const triangle_t& tria2, const plane_t& plane2) const {
    auto intersec_line = get_intersection_line(plane1, plane2);

}

line_t intersector::get_intersection_line(const plane_t& plane1, const plane_t plane2) const {
   //finding normal vector of the intersec line solving determinant square
   //------------------------------------------------------------------------------------------//
   // |a  b  c |                                                                               //  
   // |A1 B1 C1| = 0  ---> a * minor1 - b * minor2 + c * minor3 = 0 ---> solving this equation //
   // |A2 B2 C2|                                                                               //
   //------------------------------------------------------------------------------------------//
    double minor1 = determ(plane1.B_, plane1.C_, plane2.B_, plane2.C_);
    double minor2 = determ(plane1.A_, plane1.C_, plane2.A_, plane2.C_);
    double minor3 = determ(plane1.A_, plane1.B_, plane2.A_, plane2.B_);
    double a{}, b{}, c{};
    if (!is_equal(minor1, 0)) {
        a = minor2 / minor1 - minor3/ minor1;
        b = c = 1;
    } else if (!is_equal(minor2, 0)) {
        b = minor1 / minor2 + minor3/ minor2;
        a = c = 1;
    } else if (!is_equal(minor3, 0)) {
        c = minor2 / minor3 - minor1/ minor3;
        a = b = 1;
    } else {
        std::cerr << "from 'get_intersection_line()': couldn't create line\n";
    }
    
    auto line_point = solve_linear_equations({ solvePair{plane1.A_, Coeffs::A}, solvePair{plane1.B_, Coeffs::B},
                                               solvePair{plane1.C_, Coeffs::C}, solvePair{plane1.D_, Coeffs::D},
                                               solvePair{plane2.A_, Coeffs::A}, solvePair{plane2.B_, Coeffs::B},
                                               solvePair{plane2.C_, Coeffs::C}, solvePair{plane2.D_, Coeffs::D} });

}

point_t intersector::solve_linear_equations(const solveData& data) const {
    // the Gauss algorithm (matrix size is 2 * 4)
    //----------------------------------//
    // ||A1 B1 C1|-D1|| //
    // ||A2 B2 C2|-D2|| //
    // we want to get such matrix veiw:
    // ||1 0..|..|| //
    // ||0 1..|..|| //
    //----------------------------------//
    swap_first_column(data); 
    swap_second_column(data);


}

void intersector::swap_first_column(const solveData& data) const {
    static constexpr size_type FINDING_OFFSET = 3; // for defining right range for data iterator 
    static constexpr size_type COLUMN_OFFSET  = 4;

    // iterators for the up line ( A1 B1 C1 |-D1)
    auto up_begin_iter = data.begin();// points to A1
    auto first_nzero = std::find_if(up_begin_iter, up_begin_iter + FINDING_OFFSET,
                                    [](auto&& value) { return !is_equal(value.first, 0); }
                                    );
    auto column_neighbor = first_nzero + COLUMN_OFFSET;
    
    swap_columns(*first_nzero, *column_neighbor,                 // made not zero elem of
                *up_begin_iter, *(up_begin_iter + COLUMN_OFFSET)); // the first column       
}

void intersector::swap_second_column(const solveData& data) const {
    static constexpr size_type FINDING_OFFSET = 2; // for defining right range for data iterator 
    static constexpr size_type COLUMN_OFFSET  = 4;

    // iterators for the down line ( A2 B2 C2 |-D2)
    auto down_begin_iter = data.begin() + (COLUMN_OFFSET + 1); // points to B2
    auto first_nzero = std::find_if(down_begin_iter, down_begin_iter + FINDING_OFFSET,
                                    [](auto&& value) { return !is_equal(value.first, 0); }
                                    );
    auto column_neighbor = first_nzero - COLUMN_OFFSET;
    
    swap_columns(*first_nzero, *column_neighbor,                 // made not zero elem of
                *down_begin_iter, *(down_begin_iter - COLUMN_OFFSET)); // the first column       
}
// swaping two columns for linear system (matrix 2 * N)
void intersector::swap_columns(const solvePair& up_column1, const solvePair& down_column1,   // ||...  up_column1...  up_column2...||       
                  const solvePair& up_column2, const solvePair& down_column2) const {        // ||...down_column1...down_column2...||
    std::swap(up_column1, up_column2);
    std::swap(down_column1, down_column2);
}


}; // <-- namespace yLAB

