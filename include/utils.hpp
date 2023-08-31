#ifndef UTILS_
#define UTILS_

#include <limits>
#include <cmath>
#include <array>
#include <iostream>

namespace yLAB {

inline double determ(double a, double b, double c, double d) { //|a b|
    return a * d - c * b;                                      //|c d| = a * d - c * b
}

inline bool are_equal(double val1, double val2) {
    static constexpr double EPSILON = 1e-9;
    return std::fabs(val1 - val2) < EPSILON;
}

using vector_t = std::array<double, 3>; // each vector has three coordinates

inline vector_t calc_vects_product(const vector_t& vec1, const vector_t& vec2) {
    //    -> -> ->
    // ||  i  j  k ||   ->                    ->                    ->
    // || a1 b1 c1 || = i * (b1*c2 - c1*b2) - j * (a1*c2 - a2*c1) + k * (a1*b2 - b1*a2)
    // || a2 b2 c2 ||
    return { determ(vec1[1], vec1[2], vec2[1], vec2[2]),
            -determ(vec1[0], vec1[2], vec2[0], vec2[2]),
             determ(vec1[0], vec1[1], vec2[0], vec2[1]), };
}
inline bool are_complanar(const vector_t& vec1, const vector_t& vec2, const vector_t& vec3) {
    // | a1 a2 a3 |
    // | b1 b2 b3 | = 0
    // | c1 c2 c3 |
    return are_equal( vec1[0] * determ(vec2[1], vec2[2], vec3[1], vec3[2]) -
                      vec1[1] * determ(vec2[0], vec2[2], vec3[0], vec3[2]) +
                      vec1[2] * determ(vec2[0], vec2[1], vec3[0], vec3[1]), 0 );
}

inline bool is_null_vector(const vector_t& vector) {
    return are_equal(vector[0], 0) &&
           are_equal(vector[1], 0) &&
           are_equal(vector[2], 0);
}

inline vector_t get_vector(const point_t& pt1, const point_t& pt2) {
    return {(pt1.x_ - pt2.x_), (pt1.y_ - pt2.y_), (pt1.z_ - pt2.z_)};
}

inline double scalar_multiply(const vector_t& vec1, const vector_t& vec2) {            
    return vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];
}

}

#endif
