#include <gtest/gtest.h>
#include "triangle.hpp"
#include "point.hpp"
#include "utils.hpp"

using namespace yLAB;
using size_type   = std::size_t;

static constexpr size_type VARIABLES_NUMBER = 8;
static constexpr size_type SET_POINTS_SIZE  = 9;

enum class Coeffs: char {A = 0, B = 1, C = 2, D = 3}; // Ax + By + Cz + D = 0 - plane's and line's equation

using dataVal   = std::pair<triangle_t, size_type>; // saving triangle and his order number
using solvePair = std::pair<double, Coeffs>;
using solveData = std::array<solvePair, VARIABLES_NUMBER>;  // each plane contains A, B, C, D --> two planes contain 8 arguments

void swap_first_column(solveData& data) ;
void swap_second_column(solveData& data) ;
void swap_columns(solvePair& up_column1, solvePair& down_column1,
                  solvePair& up_column2, solvePair& down_column2) ; 
point_t solve_linear_equations(solveData& data)  {
// the Gauss algorithm (matrix size is 2 * 4)
//----------------------------------//
// ||A1 B1 C1|-D1|| //
// ||A2 B2 C2|-D2|| //
// we want to get such matrix veiw:
// ||1 0..|..|| //
// ||0 1..|..|| //
//----------------------------------//
swap_first_column(data);
std::cout << "after first swap:\n";
for (auto val : data) {
    std::cout << val.first << std::endl;
}
std::cout << std::endl;

std::cout << "after second swap:\n";
swap_second_column(data);
for (auto val : data) {
    std::cout << val.first << std::endl;
}
std::cout << std::endl;

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

void swap_first_column(solveData& data) {
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

void swap_second_column(solveData& data) {
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
    std::cout << "inside second swap after div\n";
    for (auto val : data) {
        std::cout << val.first << std::endl;
    }
    std::cout << std::endl;
    // if B1 != 0 --> subtract the second line from the first line with right coeff
    if (!is_equal(up_begin_iter->first, 0)) {
        double coeff = up_begin_iter->first;
        up_begin_iter->first = 0;
        for (size_type index = 2; index < COLUMN_OFFSET; ++index) {
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
void swap_columns(solvePair& up_column1, solvePair& down_column1,           // ||...  up_column1...  up_column2...||       
                               solvePair& up_column2, solvePair& down_column2) {   // ||...down_column1...down_column2...||
    std::swap(up_column1, up_column2);
    std::swap(down_column1, down_column2);
}

TEST(LinearEquation, subtest_1) {
    solveData data = { solvePair{3, Coeffs::A}, solvePair{2, Coeffs::B},
                       solvePair{1, Coeffs::C}, solvePair{7, Coeffs::D},
                       solvePair{-4, Coeffs::A}, solvePair{5, Coeffs::B},
                       solvePair{1, Coeffs::C}, solvePair{3, Coeffs::D} };
    point_t ans{1, 11./3, -52./3 };
    point_t res = solve_linear_equations(data);
    res.print();
    EXPECT_TRUE(ans == res);

}

