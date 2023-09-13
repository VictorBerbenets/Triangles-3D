#include <array>

#include "intersector.hpp"
#include "utils.hpp"
#include "point.hpp"
#include "triangle.hpp"
#include <chrono>
namespace yLAB {

bool intersector::same_intersection(const triangle_t& tria1, const triangle_t& tria2) {

    // if a point is inside the triangle
    if( check_inside_point(tria1, tria2) )          { return true; }
    if( check_inside_point(tria2, tria1) )          { return true; }
    // if at least one segment of one tria intersec another tria's segment
    if( check_segments_intersection(tria1, tria2) ) { return true; };
    return false;
}

bool intersector::check_segments_intersection(const triangle_t& tria1, const triangle_t& tria2) {
    static constexpr size_type POINTS_TO_COMPARE = 3;

    std::array<segment_t, 3> tria1_segms { segment_t {tria1[0], tria1[1]},
                                           segment_t {tria1[1], tria1[2]},
                                           segment_t {tria1[0], tria1[2]} };
    
    std::array<segment_t, 3> tria2_segms { segment_t {tria2[0], tria2[1]},
                                           segment_t {tria2[1], tria2[2]},
                                           segment_t {tria2[0], tria2[2]} };
    
    for (size_type index1 = 0; index1 < POINTS_TO_COMPARE; ++index1) {
        for (size_type index2 = 0; index2 < POINTS_TO_COMPARE; ++index2) {
            if (tria1_segms[index1].is_intersect(tria2_segms[index2])) { return true; }
        }
    }
    return false;
}

bool intersector::check_inside_point(const triangle_t& tria1, const triangle_t& tria2) {
    static constexpr size_type POINTS_TO_COMPARE = 3;

    for (size_type index1 = 0; index1 < POINTS_TO_COMPARE; ++index1) {
        size_type count = 0;
        for (size_type index2 = 0; index2 < POINTS_TO_COMPARE; ++index2) {
            count += mul_vect_products({ tria1.vertices_[index2],
                                         tria1.vertices_[(index2 + 1) % POINTS_TO_COMPARE],
                                         tria1.vertices_[(index2 + 2) % POINTS_TO_COMPARE] },
                                         tria2.vertices_[index1]);
        }
        if (count == 3) {
            return true;
        }
    }
    return false;
}

bool intersector::mul_vect_products(const triangle_t::pts_vector& tria, const point_t& comp_pt) {
    vector_t dir_tria1  {tria[0], tria[1]};
    vector_t dir_tria2  {tria[0], tria[2]};
    vector_t points_dir {tria[0], comp_pt};

    return scalar_multiply (calc_vects_product(points_dir, dir_tria1),
                            calc_vects_product(points_dir, dir_tria2)) <= 0;
}

} // <--- namespace yLAB

