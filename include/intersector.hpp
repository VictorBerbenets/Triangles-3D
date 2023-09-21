#ifndef INTERSECTOR_TYPE_
#define INTERSECTOR_TYPE_

#include <istream>
#include <vector>
#include <list>
#include <utility>
#include <array>

#include "point.hpp"
#include "line.hpp"
#include "plane.hpp"
#include "triangle.hpp"

namespace yLAB {
    
namespace intersector {
    using size_type    = std::size_t;
    using plane_coeffs = std::array<double, 4>; // A, B, C, D
    using minor_pair   = std::pair<size_type, size_type>;
    using tria_plane   = std::pair<triangle_t, plane_t>;
    
    bool are_intersecting(const triangle_t& tria1, const triangle_t& tria2);

    bool same_intersection(const triangle_t& tria1, const triangle_t& tria2);
    bool different_intersection(const tria_plane& pair1, const tria_plane& pair2);

    bool init_segment(line_t& intsec_line, const triangle_t& tria, segment_t& segm);
    bool inside_segment(const point_t& pt, const segment_t& segm);
    line_t get_intersection_line(const plane_t& plane1, const plane_t plane2);
    point_t get_planes_intersec_point(const plane_coeffs& coeffs1, const plane_coeffs& coeffs2);

    bool check_inside_point(const triangle_t& tria1, const triangle_t& tria2);
    bool check_segments_intersection(const triangle_t& tria1, const triangle_t& tria2);
    bool mul_vect_products(const triangle_t::pts_vector& tria, const point_t& comp_pt);
    minor_pair find_not_zero_minor(const plane_coeffs& coeffs1, const plane_coeffs& coeffs2);
    void find_intsec_points(std::vector<point_t>& intsec_points, line_t& intsec_line,
                                const point_t& pt1, const point_t& pt2); 
} // <--- namespace intersector

} // <--- namespace yLAB

#endif

