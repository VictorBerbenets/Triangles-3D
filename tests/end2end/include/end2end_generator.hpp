#ifndef END2END_GENERATOR_
#define END2END_GENERATOR_

#include <iostream>
#include <random>
#include <fstream>

#include "point.hpp"

namespace testing {
    using namespace yLAB;

class generator final {
    using u_int      = std::size_t;
    using gener_type = std::mt19937;

    static constexpr u_int TRIANGLES_LIMIT_NUMBER = 1000000;
    static constexpr u_int MAX_POINT_COORD        = 5000000;
    static constexpr u_int MIN_POINT_COORD        = -5000000;
    static constexpr u_int MIN_UINT_INDEX         = 0;
    static constexpr u_int MAX_UINT_INDEX         = 1;
   // static constexpr u_int ;

    
    enum class TriaOrientation: char { In_One_Plane = MIN_UINT_INDEX, In_Different_Planes = MAX_UINT_INDEX};

    void one_plane_triangles();
    void diff_planes_triangles();

    u_int random_uint() noexcept;
    double random_double() noexcept;

    point_t random_point() noexcept;
public:
    generator(std::string file_name, u_int tests_number);
    ~generator() = default;

    void generate_triangles();
private:
    std::string file_name_;
    std::ofstream test_file_;
    u_int triangles_number_;
    gener_type generator_;
};

} // <--- namespace testing

#endif

