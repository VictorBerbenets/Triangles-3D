#ifndef UTILS_
#define UTILS_

#include <limits>
#include <cmath>
#include <array>

namespace yLAB {

inline double determ(double a, double b, double c, double d) { //|a b|
    return a * d - c * b;                                      //|c d| = a * d - c * b
}

inline bool is_equal(double val1, double val2) {
    return std::fabs(val1 - val2) < std::numeric_limits<double>::epsilon();
}

using coords_t = std::array<double, 3>; // each vector has three coordinates
inline coords_t calc_vects_product(const coords_t& vec1, const coords_t& vec2) {
    //    -> -> ->
    // || i  j  k||   ->                    ->                    ->
    // ||a1 b1 c1|| = i * (b1*c2 - c1*b2) - j * (a1*c2 - a2*c1) + k * (a1*b2 - b1*a2)
    // ||a2 b2 c2||
    return { determ(vec1[1], vec1[2], vec2[1], vec2[2]),
            -determ(vec1[0], vec1[2], vec2[0], vec2[2]),
             determ(vec1[0], vec1[1], vec2[0], vec2[1]), };
}

inline double scalar_multiply(const coords_t& vec1, const coords_t& vec2) {            
    return vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];
}

}

#endif
