#ifndef INTERSECTOR_TYPE_
#define INTERSECTOR_TYPE_

#include <istream>
#include <vector>
#include <utility>

#include "point.hpp"
#include "line.hpp"
#include "plane.hpp"
#include "triangle.hpp"

namespace yLAB {

class intersector {
    using size_type   = std::size_t;
    
    static constexpr size_type VARIABLES_NUMBER = 8;
    static constexpr size_type SET_POINTS_SIZE  = 9;

    enum class Coeffs: char {A = 0, B = 1, C = 2, D = 3}; // Ax + By + Cz + D = 0 - plane's and line's equation
    
    using dataVal   = std::pair<triangle_t, size_type>; // saving triangle and his order number
    using solvePair = std::pair<double, Coeffs>;
    using solveData = std::array<solvePair, VARIABLES_NUMBER>;  // each plane contains A, B, C, D --> two planes contain 8 arguments

//--------------------------------triangles in different planes--------------------------------//
    bool different_intersection(const triangle_t& tria1, const triangle_t& tria2) const;

    line_t get_intersection_line(const plane_t& plane1, const plane_t plane2) const;
    segment_t get_segment(line_t& intsec_line, const triangle_t& tria) const;
    void find_intsec_points(std::vector<point_t>& intsec_points, line_t& intsec_line,
                                    const point_t& pt1, const point_t& pt2) const;
    bool inside_segment(const point_t& pt, const segment_t& segm) const;
    point_t solve_linear_equations(solveData& data) const;

    void swap_first_column(solveData& data) const;
    void swap_second_column(solveData& data) const;
    void swap_columns(solvePair& up_column1, solvePair& down_column1,
                      solvePair& up_column2, solvePair& down_column2) const; 
//--------------------------------triangles in one plane--------------------------------------//

public:
    intersector(std::istream& stream);
    ~intersector() = default;

    void print_intersected_triangles() const;
private:
    std::vector<dataVal> data_;
};

} 

#endif
