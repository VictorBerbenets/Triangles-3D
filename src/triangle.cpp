#include <iostream>
#include <algorithm>
#include <vector>

#include "triangle.hpp"

namespace yLAB {

namespace {
    using size_type    = std::size_t;
    using tria_plane   = std::pair<triangle_t, plane_t>;

    bool mul_vect_products(const triangle_t::pts_vector& tria, const point_t& comp_pt) {
        vector_t dir_tria1  {tria[0], tria[1]};
        vector_t dir_tria2  {tria[0], tria[2]};
        vector_t points_dir {tria[0], comp_pt};

        return scalar_multiply (calc_vects_product(points_dir, dir_tria1),
                                calc_vects_product(points_dir, dir_tria2)) <= 0;
    }

    void find_intsec_points(std::vector<point_t>& intsec_points, line_t& intsec_line,
                                const point_t& pt1, const point_t& pt2) {
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

    bool init_segment(line_t& intsec_line, const triangle_t& tria, segment_t& segm) {
        std::vector<point_t> intsec_points{};
        for (int index = 0; index < 3; ++index) {
            find_intsec_points(intsec_points, intsec_line, tria[index], tria[(index + 1) % 3]);
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

    bool different_intersection(const tria_plane& pair1, const tria_plane& pair2) {
        line_t intsec_line = pair1.second.get_intersec_line(pair2.second);
        
        segment_t tr1_segment{};
        if (!init_segment(intsec_line, pair1.first, tr1_segment)) { return false; }
        segment_t tr2_segment{};
        if (!init_segment(intsec_line, pair2.first, tr2_segment)) { return false; }
        
        return tr1_segment.is_intersect(tr2_segment);   
    }

} // <--- local namespace

/*-------------------------------------triangles interface-------------------------------------------*/
triangle_t::triangle_t(const point_t& pt1, const point_t& pt2, const point_t& pt3):
    vertices_{pt1, pt2, pt3} {};

triangle_t::triangle_t( double val1, double val2, double val3,
                        double val4, double val5, double val6,
                        double val7, double val8, double val9 ):
    vertices_{ point_t{val1, val2, val3},
               point_t{val4, val5, val6},
               point_t{val7, val8, val9} } {};

bool triangle_t::is_intersecting(const triangle_t& rhs) const {
    tria_plane pair1 = {*this, get_plane()};
    tria_plane pair2 = {rhs, rhs.get_plane()};

    if (!pair1.second.is_parallel(pair2.second)) {
        return different_intersection(pair1, pair2);
    } else if (pair1.second.contains(pair2.second.get_plane_point())) { // if planes are parallel and one plane contains                                                                               
        return pair1.first.same_intersection(pair2.first);             // a point of another plane --> they are the same
    }
    return false;
}

bool triangle_t::same_intersection(const triangle_t& rhs) const {
    const auto lhs = *this;
    // if a point is inside the triangle
    if(lhs.check_inside_point(rhs)) { return true; }
    if(rhs.check_inside_point(lhs)) { return true; }
    // if at least one segment of one tria intersec another tria's segment
    if(lhs.check_segments_intersection(rhs)) { return true; };
    return false;

}

bool triangle_t::check_inside_point(const triangle_t& rhs) const {
    static constexpr size_type POINTS_TO_COMPARE = 3;
    const auto& lhs = *this;

    for (size_type index1 = 0; index1 < POINTS_TO_COMPARE; ++index1) {
        size_type count = 0;
        for (size_type index2 = 0; index2 < POINTS_TO_COMPARE; ++index2) {
            if (mul_vect_products({ lhs[index2],
                                    lhs[(index2 + 1) % POINTS_TO_COMPARE],
                                    lhs[(index2 + 2) % POINTS_TO_COMPARE] },
                                    rhs[index1]) ) {
                ++count;
            } else { break; }
        }
        if (count == 3) { return true; }
    }
    return false;
}

bool triangle_t::check_segments_intersection(const triangle_t& rhs) const {
    static constexpr size_type POINTS_TO_COMPARE = 3;
    const auto& lhs = *this;

    std::array<segment_t, 3> lhs_segms { segment_t {lhs[0], lhs[1]},
                                         segment_t {lhs[1], lhs[2]},
                                         segment_t {lhs[0], lhs[2]} };
    
    std::array<segment_t, 3> rhs_segms { segment_t {rhs[0], rhs[1]},
                                         segment_t {rhs[1], rhs[2]},
                                         segment_t {rhs[0], rhs[2]} };
    
    for (size_type index1 = 0; index1 < POINTS_TO_COMPARE; ++index1) {
        for (size_type index2 = 0; index2 < POINTS_TO_COMPARE; ++index2) {
            if (lhs_segms[index1].is_intersect(rhs_segms[index2])) { return true; }
        }
    }
    return false;
}

plane_t triangle_t::get_plane() const {
    return {vertices_[0], vertices_[1], vertices_[2]};
}

const point_t& triangle_t::operator[](std::size_t index) const {
    return vertices_[index];
}

point_t& triangle_t::operator[](std::size_t index) {
    return vertices_[index];
}

std::ostream& operator<<(std::ostream& os, const triangle_t& tria) {
    os << tria[0] << std::endl;
    os << tria[1] << std::endl;
    return os << tria[2];
}

} // <--- namespace yLAB
