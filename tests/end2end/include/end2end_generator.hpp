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

namespace dirrs {
    const std::string res_dir   = "../tests/end2end/resources";
    const std::string tests_dir = "../tests/end2end/resources/tests/";
    //const std::string answ_dir  = "../../tests/end2end/resources/answers/";
}

class generator final {
public:
    using u_int          = std::size_t;
    using gener_type     = std::mt19937;
    using trias_confines = std::array<int, 2>;
    
    static constexpr u_int TRIANGLES_POINTS = 3;
    
    static constexpr u_int SPACE_LIMIT            = 1000000;
    static constexpr u_int TRIANGLES_LIMIT_NUMBER = 1000000;
    static constexpr int MAX_POINT_COORD          = 50000;
    static constexpr int MIN_POINT_COORD          = -50000;
    static constexpr u_int MIN_TRIA_SIDE          = 5;
    static constexpr u_int MAX_TRIA_SIDE          = 1000;
private:
   // static constexpr u_int ;
    point_t random_point();
    double random_float(double min_val = MIN_POINT_COORD, double max_val = MAX_POINT_COORD);
    u_int random_uint(u_int min_val = MIN_TRIA_SIDE, u_int max_val = MAX_TRIA_SIDE);
public:
    generator(std::string file_name, u_int tests_number,/* double space_confine,*/
              int min_tria_side = MIN_TRIA_SIDE, int max_tria_side = MAX_TRIA_SIDE);
    ~generator() = default;
    
    void generate_points();
private:
    std::string file_name_;
    std::ofstream test_file_;
    u_int triangles_number_;
    gener_type generator_;
    
    //double sp_confine_;
    trias_confines tr_confines_;
};

} // <--- namespace testing

#endif

