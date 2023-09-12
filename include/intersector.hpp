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
    
class intersector final {
public:
    using size_type    = std::size_t;
    using plane_coeffs = std::array<double, 4>; // A, B, C, D
    using minor_pair   = std::pair<size_type, size_type>;
    using tria_plane   = std::pair<triangle_t, plane_t>;
    
    static bool are_intersecting(const triangle_t& tria1, const triangle_t& tria2);

    static bool same_intersection(const triangle_t& tria1, const triangle_t& tria2);
    static bool different_intersection(const tria_plane& pair1, const tria_plane& pair2);

    static bool init_segment(line_t& intsec_line, const triangle_t& tria, segment_t& segm);
    static bool inside_segment(const point_t& pt, const segment_t& segm);
    static line_t get_intersection_line(const plane_t& plane1, const plane_t plane2);
    static point_t get_planes_intersec_point(const plane_coeffs& coeffs1, const plane_coeffs& coeffs2);
private:
    static bool mul_vect_products(const triangle_t::pts_vector& tria, const point_t& comp_pt);
    static minor_pair find_not_zero_minor(const plane_coeffs& coeffs1, const plane_coeffs& coeffs2);
    static void find_intsec_points(std::vector<point_t>& intsec_points, line_t& intsec_line,
                                    const point_t& pt1, const point_t& pt2); 
};

} // <--- namespace yLAB

#endif

