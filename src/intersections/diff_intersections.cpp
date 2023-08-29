#include <iostream>
#include <vector>
#include <utility>

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
    vector_t line_vec = intsec_line.get_dirr_vec();
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
    return is_equal(segm.length(), segm_part1.length() + segm_part2.length());
}

line_t intersector::get_intersection_line(const plane_t& plane1, const plane_t plane2) const {
    return {plane1.A_ - plane2.A_, plane1.B_ - plane2.B_, plane1.C_ - plane2.C_, plane1.D_ - plane2.D_};
}

point_t intersector::solve_linear_equations(solveData& data) const {
    // the Gauss algorithm (matrix size is 2 * 4)
    //----------------------------------//
    // ||A1 B1 C1|-D1|| //
    // ||A2 B2 C2|-D2|| //
    // we want to get such matrix veiw:
    // ||1 0..|..|| //
    // ||0 1..|..|| //
    //----------------------------------//
    swap_first_column(data);
    swap_second_column(data);
    // after reduction to diagonal form finding point's coords
    double x{}, y{}, z{}; // point coordinates
    auto first_it  = data.begin();
    auto second_it = first_it + 5;
    double coord1  = -(data[2].first + data[3].first);
    double coord2  = -(data[6].first + data[7].first);
    char error_key = static_cast<char>(second_it->second);
    switch(first_it->second) {
        case Coeffs::A:
            switch(second_it->second) {
                case Coeffs::B:
                    x = coord1;
                    y = coord2;
                    z = 1;
                    break;
                case Coeffs::C:
                    x = coord1;
                    y = 1;
                    z = coord2;
                    break;
                default:
                    std::cerr << "invalid key: " << error_key <<
                        ". Expected B or C\n";
            }
            break;
        case Coeffs::B:
            switch(second_it->second) {
                case Coeffs::A:
                    x = coord2;
                    y = coord1;
                    z = 1;
                    break;
                case Coeffs::C:
                    x = 1;
                    y = coord1;
                    z = coord2;
                    break;
                default: std::cerr << "invalid key: " << error_key <<
                        ". Expected A or C\n";
            }
            break;
        case Coeffs::C:
            switch(second_it->second) {
                case Coeffs::A:
                    x = coord2;
                    y = 1;
                    z = coord1;
                    break;
                case Coeffs::B:
                    x = 1;
                    y = coord2;
                    z = coord1;
                    break;
                default: std::cerr << "invalid key: " << error_key <<
                     ". Expected A or B\n";
            }
            break;
        default: std::cerr << "invalid key: " << static_cast<char>(first_it->second) <<
               ". Expected A, B or C\n";
    }
    return {x, y, z};
}

void intersector::swap_first_column(solveData& data) const {
    static constexpr size_type FINDING_OFFSET = 3; // for defining right range for data iterator 
    static constexpr size_type COLUMN_OFFSET  = 4;

    // iterators for the up line ( A1 B1 C1 |-D1)
    auto up_begin_iter = data.begin();                   // points to A1
    auto up_end_iter   = up_begin_iter + FINDING_OFFSET; // points to -D1
    auto first_nzero   = std::find_if(up_begin_iter, up_end_iter,
                                    [](auto&& value) { return !is_equal(value.first, 0); }
                                    );
    auto column_neighbor = first_nzero + COLUMN_OFFSET;   
    auto down_begin_iter = up_begin_iter + COLUMN_OFFSET; //points to A2
    swap_columns(*first_nzero, *column_neighbor, 
                *up_begin_iter, *down_begin_iter); 
    //  normalization of the first line 
    for (auto iter = up_begin_iter; iter <= up_end_iter; ++iter) {
        iter->first /= first_nzero->first;
    }
    // if A2 != 0 --> subtract the first line from the second line with right coeff
    if (!is_equal(down_begin_iter->first, 0)) {
        double coeff = down_begin_iter->first;
        down_begin_iter->first = 0;
        for (size_type index = 1; index < COLUMN_OFFSET; ++index) {
            data[index + COLUMN_OFFSET].first -= data[index].first * coeff;
        }
    }
    //-----------------//
    // now we have:
    // ||1 .. ..|..||
    // ||0 .. ..|..||
    //-----------------//
}

void intersector::swap_second_column(solveData& data) const {
    static constexpr size_type FINDING_OFFSET = 2; // for defining right range for data iterator
    static constexpr size_type COLUMN_OFFSET  = 4;

    // iterators for the down line ( 0 B2 C2 |-D2)
    auto down_begin_iter = data.begin() + (COLUMN_OFFSET + 1);  // points to B2
    auto down_end_iter   = down_begin_iter + FINDING_OFFSET;    // points to -D2
    auto first_nzero     = std::find_if(down_begin_iter, down_end_iter,
                                    [](auto&& value) { return !is_equal(value.first, 0); }
                                    );
    auto column_neighbor = first_nzero - COLUMN_OFFSET;
    auto up_begin_iter   = data.begin() + 1; // points to B1
    swap_columns(*first_nzero, *column_neighbor,
                *down_begin_iter, *up_begin_iter);
    //  normalization of the second line 
    for (auto iter = down_begin_iter; iter <= down_end_iter; ++iter) {
        iter->first /= first_nzero->first;
    }
    // if B1 != 0 --> subtract the second line from the first line with right coeff
    if (!is_equal(up_begin_iter->first, 0)) {
        double coeff = up_begin_iter->first;
        up_begin_iter->first = 0;
        for (size_type index = 1; index < COLUMN_OFFSET; ++index) {
            data[index].first -= data[index + COLUMN_OFFSET].first * coeff;
        }
    }
    //-----------------//
    // now we have:
    // ||1 0 ..|..||
    // ||0 1 ..|..||
    //-----------------//
}
// swaping two columns for linear system (matrix 2 * N)
void intersector::swap_columns(solvePair& up_column1, solvePair& down_column1,           // ||...  up_column1...  up_column2...||       
                               solvePair& up_column2, solvePair& down_column2) const {   // ||...down_column1...down_column2...||
    std::swap(up_column1, up_column2);
    std::swap(down_column1, down_column2);
}

}

