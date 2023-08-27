#include "intersector.hpp"

namespace yLAB {

intersector::printPair intersector::different_intersection(const triangle_t& tria1, const plane_t& plane1,
                                     const triangle_t& tria2, const plane_t& plane2) const {
    auto intersec_line = get_intersection_line(plane1, plane2);

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
    double a{}, b{}, c{};
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
    
    auto line_point = solve_linear_equations({ solvePair{plane1.A_, A}, solvePair{plane1.B_, B},
                                               solvePair{plane1.C_, C}, solvePair{plane1.D_, D},
                                               solvePair{plane2.A_, A}, solvePair{plane2.B_, B},
                                               solvePair{plane2.C_, C}, solvePair{plane2.D_, D} });

}

point_t intersector::solve_linear_equations(const solveData& data) const {
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


}

void intersector::swap_first_column(const solveData& data) const {
    static constexpr size_type FINDING_OFFSET = 3; // for defining right range for data iterator 
    static constexpr size_type COLUMN_OFFSET  = 4;

    // iterators for the up line ( A1 B1 C1 |-D1)
    auto up_begin_iter = data.begin();// points to A1
    auto first_nzero = std::find_if(up_begin_iter, up_begin_iter + FINDING_OFFSET,
                                    [](auto&& value) { return !is_equal(value.first, 0); }
                                    );
    auto column_neighbor = first_nzero + COLUMN_OFFSET;
    
    swap_columns(*first_nzero, *column_neighbor,                 // made not zero elem of
                *up_begin_iter, *(up_begin_iter + COLUMN_OFFSET)); // the first column       
}

void intersector::swap_second_column(const solveData& data) const {
    static constexpr size_type FINDING_OFFSET = 2; // for defining right range for data iterator 
    static constexpr size_type COLUMN_OFFSET  = 4;

    // iterators for the down line ( A2 B2 C2 |-D2)
    auto down_begin_iter = data.begin() + (COLUMN_OFFSET + 1); // points to B2
    auto first_nzero = std::find_if(down_begin_iter, down_begin_iter + FINDING_OFFSET,
                                    [](auto&& value) { return !is_equal(value.first, 0); }
                                    );
    auto column_neighbor = first_nzero - COLUMN_OFFSET;
    
    swap_columns(*first_nzero, *column_neighbor,                 // made not zero elem of
                *down_begin_iter, *(down_begin_iter - COLUMN_OFFSET)); // the first column       
}
// swaping two columns for linear system (matrix 2 * N)
void intersector::swap_columns(const solvePair& up_column1, const solvePair& down_column1,   // ||...  up_column1...  up_column2...||       
                  const solvePair& up_column2, const solvePair& down_column2) const {        // ||...down_column1...down_column2...||
    std::swap(up_column1, up_column2);
    std::swap(down_column1, down_column2);
}

};

