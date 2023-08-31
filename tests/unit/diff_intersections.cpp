#include <gtest/gtest.h>

#include "intersector.hpp"
#include "utils.hpp"

using namespace yLAB;

//Copied methods from 'intersector.hpp' file for checking 'get_planes_intersec_point()' func working //
using size_type   = std::size_t;
using dataVal      = std::pair<triangle_t, size_type>; // saving triangle and his order number
using plane_coeffs = std::array<double, 4>; // A, B, C, D
using minor_pair   = std::pair<size_type, size_type>;


line_t get_intersection_line(const plane_t& plane1, const plane_t plane2);
point_t get_planes_intersec_point(const plane_coeffs& coeffs1, const plane_coeffs& coeffs2);
minor_pair find_not_zero_minor(const plane_coeffs& coeffs1, const plane_coeffs& coeffs2);

line_t get_intersection_line(const plane_t& plane1, const plane_t plane2)  {
    vector_t dirr_vec = calc_vects_product(plane1.get_coords(), plane2.get_coords());
    point_t intsec_pt = get_planes_intersec_point( plane_coeffs{plane1.A_, plane1.B_, plane1.C_, plane1.D_},
                                  plane_coeffs{plane2.A_, plane2.B_, plane2.C_, plane2.D_} );
    if (!intsec_pt.is_valid()) {
        throw std::runtime_error{"Couldn't get intersection point\n"};
    }
    return {dirr_vec, intsec_pt};
}

point_t get_planes_intersec_point(const plane_coeffs& coeffs1, const plane_coeffs& coeffs2){
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
minor_pair find_not_zero_minor(const plane_coeffs& coeffs1, const plane_coeffs& coeffs2) {
    for (size_type index = 0; index < 3; ++index) {
        size_type next_index = (index + 1) % 3;
        double det = determ( coeffs1[index], coeffs1[next_index], coeffs2[index], coeffs2[next_index] );
        if (!are_equal(det, 0)) {
            return {std::min(index, next_index), std::max(index, next_index)};
        }
    }
    return {NAN, NAN};
}
//---------------------------------------------------------------------------------------------------------//

TEST(Different_Planes_Intersection, Get_Intersection_line) {
    plane_t pl1{2, 1, -1, -1};
    plane_t pl2{1, 3, -2, 0};
    
    line_t intsec_line1 = get_intersection_line(pl1, pl2);
    EXPECT_TRUE((intsec_line1 == line_t{1, 3, 5, point_t{1, 1, 2}}));
    
    plane_t pl3{1, 0, 0, 0};
    plane_t pl4{0, 0, 1, 0};
    
    line_t intsec_line2 = get_intersection_line(pl3, pl4);
    EXPECT_TRUE((intsec_line2 == line_t{0, -100, 0, point_t{0, 1789, 0}}));

    plane_t pl5{1, 0, 0, 0};
    plane_t pl6{0, 1, 0, 0};
    
    line_t intsec_line3 = get_intersection_line(pl5, pl6);
    EXPECT_TRUE((intsec_line3 == line_t{0, 0, -9532952.5235, point_t{0, 0, -532523.532523}}));

}
