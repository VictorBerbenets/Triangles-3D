#ifndef END2END_GENERATOR_
#define END2END_GENERATOR_

#include <iostream>
#include <random>
#include <fstream>
#include <set>

#include "point.hpp"
#include "triangle.hpp"

namespace testing {
    using namespace yLAB;

class generator final {
public:
    using u_int          = std::size_t;
    using gener_type     = std::mt19937;
    using trias_confines = std::array<u_int, 2>;

    static constexpr u_int TRIANGLES_LIMIT_NUMBER = 1000000;
    static constexpr u_int MAX_POINT_COORD        = 50000;
    static constexpr u_int MIN_POINT_COORD        = -500000;
    static constexpr u_int MIN_TRIA_SIDE          = 5;
    static constexpr u_int MAX_TRIA_SIDE          = 1000;

   // static constexpr u_int ;

public:
    generator(std::string file_name, u_int tests_number, double space_confine,
              u_int min_tria_side = MIN_TRIA_SIDE, u_int max_tria_side = MAX_TRIA_SIDE);
    ~generator() = default;

    void generate_triangles();
private:
    std::string file_name_;
    std::ofstream test_file_;
    u_int triangles_number_;
    
    double sp_confine_;
    trias_confines tr_confines_;
};

} // <--- namespace testing

#endif

