#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <stdexcept>

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
    vector_t dirr_vec = calc_vects_product(plane1.get_coords(), plane2.get_coords());
    point_t intsec_pt = get_planes_intersec_point( plane_coeffs{plane1.A_, plane1.B_, plane1.C_, plane1.D_},
                                  plane_coeffs{plane2.A_, plane2.B_, plane2.C_, plane2.D_} );
    if (!intsec_pt.is_valid()) {
        throw std::runtime_error{"Couldn't get intersection point\n"};
    }
    return {dirr_vec, intsec_pt};
}

point_t intersector::get_planes_intersec_point(const plane_coeffs& coeffs1, const plane_coeffs& coeffs2) const {
    static constexpr int  ARBITRARY_VALUE = 1;

    auto [colum1, colum2] = find_not_zero_minor(coeffs1, coeffs2);
    if (std::isnan(colum1) || std::isnan(colum2)) {
        throw std::runtime_error{"Couldn't find not zero minor\n"};
    }
    double not_zero_minor = determ( coeffs1[colum1], coeffs1[colum2],
                                    coeffs2[colum1], coeffs2[colum2] );
    size_type arbit_col = 3 - (colum1 + colum2); //colum1 + colum2 <=3
    double det_colum1 = determ(-coeffs1[3] - ARBITRARY_VALUE * coeffs1[arbit_col], coeffs1[colum2],
                        -coeffs2[3] - ARBITRARY_VALUE * coeffs2[arbit_col], coeffs2[colum2] );
    double det_colum2 = determ( coeffs1[colum1], -coeffs1[3] - ARBITRARY_VALUE * coeffs1[arbit_col],
                         coeffs2[colum1], -coeffs2[3] - ARBITRARY_VALUE * coeffs2[arbit_col] );
    using pair = std::pair<double, size_type>;
    std::array<pair, 3> determ_and_id = { pair{det_colum1, colum1},
                                          pair{det_colum2, colum2},
                                          pair{ARBITRARY_VALUE, arbit_col} };
    std::sort( determ_and_id.begin(), determ_and_id.end(),
                         [](auto&& val1, auto&& val2) { return val1.second < val2.second; }
             );
    return { 0 == arbit_col ? ARBITRARY_VALUE : determ_and_id[0].first / not_zero_minor,
             1 == arbit_col ? ARBITRARY_VALUE : determ_and_id[1].first / not_zero_minor,
             2 == arbit_col ? ARBITRARY_VALUE : determ_and_id[2].first / not_zero_minor };
}
intersector::minor_pair intersector::find_not_zero_minor(const plane_coeffs& coeffs1, const plane_coeffs& coeffs2) const {
    for (size_type index = 0; index < 3; ++index) {
        size_type next_index = (index + 1) % 3;
        double det = determ( coeffs1[index], coeffs1[next_index], coeffs2[index], coeffs2[next_index] );
        if (!are_equal(det, 0)) {
            return {std::min(index, next_index), std::max(index, next_index)};
        }
    }
    return {NAN, NAN};
}

}
