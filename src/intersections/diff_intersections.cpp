#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

#include "intersector.hpp"
#include "utils.hpp"
#include "point.hpp"
#include "line.hpp"
#include "plane.hpp"
#include "triangle.hpp"

namespace yLAB {
//bool intersector::
bool intersector::different_intersection(const triangle_t& tria1, const triangle_t& tria2) const {
    line_t intsec_line = get_intersection_line(tria1.get_plane(), tria2.get_plane());
    
    // finding intersection between triangles
    segment_t tr1_segment = get_segment(intsec_line, tria1);
    if (!tr1_segment.is_valid()) {
        return false;
    }
    segment_t tr2_segment = get_segment(intsec_line, tria2);
    if (!tr2_segment.is_valid()) {
        return false;
    }

}

segment_t intersector::get_segment(line_t& intsec_line, const triangle_t& tria) const {
    std::vector<point_t> intsec_points{};
    for (int index = 0; index < 3; ++index) {
        find_intsec_points(intsec_points, intsec_line, tria.vertices_[index], tria.vertices_[(index + 1) % 3]);
    }
    if (intsec_points.size() == 1) {
        return {intsec_points.front()};
    }
    if (intsec_points.size() == 2) {
        return {intsec_points.front(), intsec_points.back()};
    }
    return {};
}

void intersector::find_intsec_points(std::vector<point_t>& intsec_points, line_t& intsec_line,
                                    const point_t& pt1, const point_t& pt2) const {
    vector_t line_vec = intsec_line.dir_coords();
    vector_t segm_vec = get_vector(pt1, pt2);
    
    if (is_null_vector(calc_vects_product(line_vec, segm_vec))) {
        // if both parallels each other and line contains one segment's point
        // then sigment lies on the line
        if (intsec_line.contains(pt1)) {
            intsec_points.insert(intsec_points.end(), {pt1, pt2});
        }
    } else {
        point_t common_point = intsec_line.get_intersec_point({pt1, pt2}); 
        if (inside_segment(common_point, {pt1, pt2})) {
            intsec_points.push_back(common_point);
        }
    }
}

bool intersector::inside_segment(const point_t& pt, const segment_t& segm) const {
    segment_t segm_part1(pt, segm.pt1_);
    segment_t segm_part2(pt, segm.pt2_);
    return are_equal(segm.length(), segm_part1.length() + segm_part2.length());
}

line_t intersector::get_intersection_line(const plane_t& plane1, const plane_t plane2) const {
//    return {plane1.A_ - plane2.A_, plane1.B_ - plane2.B_, plane1.C_ - plane2.C_, plane1.D_ - plane2.D_};
    return {1, 2, 3, {1, 2, 3}};
}

}
