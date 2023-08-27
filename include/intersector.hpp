#ifndef INTERSECTOR_TYPE_
#define INTERSECTOR_TYPE_

#include <iostream>
#include <vector>
#include <unordered_set>

#include "utils.hpp"
#include "point.hpp"
#include "line.hpp"
#include "plane.hpp"
#include "triangle.hpp"

namespace yLAB {

class intersector {
    using size_type   = std::size_t;
    
    static constexpr size_type VARIABLES_NUMBER = 8;
    static constexpr size_type SET_POINTS_SIZE  = 9;

    enum Coeffs: char {A = 0, B = 1, C = 2, D = 3}; // Ax + By + Cz + D = 0 - plane's equation

    using data_val    = std::pair<triangle_t, size_type>; // saving triangle and his order number
    using printPair   = std::pair<size_type, size_type>;
    using solvePair   = std::pair<double, char>;
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

}; 

#endif
