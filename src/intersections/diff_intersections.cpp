#include "intersector.hpp"

namespace yLAB {

intersector::printPair intersector::different_intersection(const triangle_t& tria1, const triangle_t& tria2) const {
    auto intersec_line = get_intersection_line(tria1.get_plane(), tria2.get_plane());
    

}

line_t intersector::get_intersection_line(const plane_t& plane1, const plane_t plane2) const {
   //finding normal vector of the intersec line solving determinant square
   //------------------------------------------------------------------------------------------//
   // |a  b  c |                                                                               //  
   // |A1 B1 C1| = 0  ---> a * minor1 - b * minor2 + c * minor3 = 0 ---> solving this equation //
   // |A2 B2 C2|                                                                               //
   //------------------------------------------------------------------------------------------//
    double minor1 = determ(plane1.B_, plane1.C_, plane2.B_, plane2.C_);
    double minor2 = determ(plane1.A_, plane1.C_, plane2.A_, plane2.C_);
    double minor3 = determ(plane1.A_, plane1.B_, plane2.A_, plane2.B_);
    double a{}, b{}, c{}, d{}; // line's components
    if (!is_equal(minor1, 0)) {
        a = minor2 / minor1 - minor3/ minor1;
        b = c = 1;
    } else if (!is_equal(minor2, 0)) {
        b = minor1 / minor2 + minor3/ minor2;
        a = c = 1;
    } else if (!is_equal(minor3, 0)) {
        c = minor2 / minor3 - minor1/ minor3;
        a = b = 1;
    } else {
        std::cerr << "from 'get_intersection_line()': couldn't create line\n";
    }
    solveData data = { solvePair{plane1.A_, Coeffs::A}, solvePair{plane1.B_, Coeffs::B},
                       solvePair{plane1.C_, Coeffs::C}, solvePair{plane1.D_, Coeffs::D},
                       solvePair{plane2.A_, Coeffs::A}, solvePair{plane2.B_, Coeffs::B},
                       solvePair{plane2.C_, Coeffs::C}, solvePair{plane2.D_, Coeffs::D} };
    // getting a point that belongs to both planes --> belongs our line
    point_t point = solve_linear_equations(data);
    //finding d line coeff (ax + by + cz + d = 0 - line equation)
    d = -(a * point.x_ + b * point.y_ + c * point.z_);
    return {a, b, c, d};                                                                      
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
    double coord1 = -(data[2].first + data[3].first);
    double coord2 = -(data[6].first + data[7].first);
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
                    std::cerr << "invalid key: " << static_cast<char>(second_it->second) <<
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
                default: std::cerr << "invalid key: " << static_cast<char>(second_it->second) <<
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
                default: std::cerr << "invalid key: " << static_cast<char>(second_it->second) <<
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

};

