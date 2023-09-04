#ifndef INTERSECTOR_TYPE_
#define INTERSECTOR_TYPE_

#include <istream>
#include <vector>
#include <utility>
#include <array>

#include "oct_tree.hpp"
#include "point.hpp"
#include "line.hpp"
#include "plane.hpp"
#include "triangle.hpp"

namespace yLAB {
    using spaceBreaking::OctTree;
class intersector final {
public:
    using size_type    = std::size_t;
    using dataVal      = std::pair<triangle_t, size_type>; // saving triangle and his order number
    using plane_coeffs = std::array<double, 4>; // A, B, C, D
    using minor_pair   = std::pair<size_type, size_type>;
private:
    using tria_plane = std::pair<triangle_t, plane_t>;
    
    static constexpr size_type SET_POINTS_SIZE  = 9;
//--------------------------------triangles in the one plane-----------------------------------//
    bool same_intersection(const triangle_t& tria1, const triangle_t& tria2) const;
    bool mul_vect_products(const triangle_t::pts_vector& tria, const point_t& comp_pt) const;
    
//--------------------------------triangles in different planes--------------------------------//
    bool different_intersection(const tria_plane& pair1, const tria_plane& pair2) const;
    bool init_segment(line_t& intsec_line, const triangle_t& tria, segment_t& segm) const;
    bool inside_segment(const point_t& pt, const segment_t& segm) const;

    line_t get_intersection_line(const plane_t& plane1, const plane_t plane2) const;
    point_t get_planes_intersec_point(const plane_coeffs& coeffs1, const plane_coeffs& coeffs2) const;
    minor_pair find_not_zero_minor(const plane_coeffs& coeffs1, const plane_coeffs& coeffs2) const;
    void find_intsec_points(std::vector<point_t>& intsec_points, line_t& intsec_line,
                                    const point_t& pt1, const point_t& pt2) const; 
//--------------------------------triangles in one plane--------------------------------------//

public:
    intersector(std::istream& stream);
    ~intersector() = default;

    void print_intersected_triangles() const;
private:
    std::vector<dataVal> data_;
    OctTree oct_tree_;
};

} // <--- namespace yLAB

#endif

