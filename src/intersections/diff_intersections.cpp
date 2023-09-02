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

#include <chrono>
namespace yLAB {

bool intersector::different_intersection(const tria_plane& pair1, const tria_plane& pair2) const {
    line_t intsec_line = get_intersection_line(pair1.second, pair2.second);
    
    segment_t tr1_segment{};
    if (!init_segment(intsec_line, pair1.first, tr1_segment)) { return false; }
    segment_t tr2_segment{};
    if (!init_segment(intsec_line, pair2.first, tr2_segment)) { return false; }
    
    return tr1_segment.is_intersect(tr2_segment);    
}

bool intersector::init_segment(line_t& intsec_line, const triangle_t& tria, segment_t& segm) const {
    std::vector<point_t> intsec_points{};
    for (int index = 0; index < 3; ++index) {
        find_intsec_points(intsec_points, intsec_line, tria.vertices_[index], tria.vertices_[(index + 1) % 3]);
        if (intsec_points.size() == 2) {
            segm.set_ends(intsec_points.front(), intsec_points.back());
            return true;
        }
    }
    if (intsec_points.size() == 1) {
        segm.set_ends(intsec_points.front());
        return true;
    } 
    return false;
}

void intersector::find_intsec_points(std::vector<point_t>& intsec_points, line_t& intsec_line,
                                    const point_t& pt1, const point_t& pt2) const {
    auto push_back_if = [&intsec_points](const point_t& pt) {
                            if (std::find(intsec_points.begin(), intsec_points.end(), pt) == intsec_points.end()) {
                                intsec_points.push_back(pt);
                            }
                        };
    if ( calc_vects_product(intsec_line.dir_coords_, {pt1, pt2}).is_null() ) {
        // if both parallels each other and line contains one segment's point
        // then sigment lies on the line
        if (intsec_line.contains(pt1)) {
            push_back_if(pt1);
            push_back_if(pt2);
        }
    } else {
        point_t common_point = intsec_line.get_intersec_point({pt1, pt2});
        segment_t segm {pt1, pt2};
        if (segm.is_inside(common_point)) {
            push_back_if(common_point);
        }
    }
}

line_t intersector::get_intersection_line(const plane_t& plane1, const plane_t plane2) const {
    vector_t dirr_vec = calc_vects_product(plane1.get_coords(), plane2.get_coords());
    point_t intsec_pt = get_planes_intersec_point( plane_coeffs{plane1.normal_coords_[0], plane1.normal_coords_[1], plane1.normal_coords_[2], plane1.D_},
                                                   plane_coeffs{plane2.normal_coords_[0], plane2.normal_coords_[1], plane2.normal_coords_[2], plane2.D_} );
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
