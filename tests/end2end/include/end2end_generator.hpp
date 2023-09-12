#ifndef END2END_GENERATOR_
#define END2END_GENERATOR_

#include <iostream>
#include <random>
#include <fstream>

namespace testing {

class generator final {
    using u_int      = std::size_t;
    using gener_type = std::mt19937;

    static constexpr u_int TRIANGLES_LIMIT_NUMBER = 1000000;
    static constexpr u_int MAX_POINT_COORD        = 5000000;
    static constexpr u_int MIN_POINT_COORD        = -5000000;
#if 0   
    static constexpr u_int ;
    static constexpr u_int ;
    static constexpr u_int ;
#endif
    
    enum class TriaOrientation: char { In_One_Plane = 0, In_Different_Planes = 1};

    void one_plane_triangles() const;
    void diff_planes_triangles() const;

    void init_generator(gener_type& gener) const;
    u_int random_uint(gener_type& generator) const;
    double random_double(gener_type& generator) const;
public:
    generator(std::string file_name, u_int tests_number):
              test_file_{file_name},
              file_name_{file_name},
              triangles_number_{tests_number} {}
    ~generator() = default;

    void generate_triangles() const;
private:
    std::ofstream test_file_;
    std::string file_name_;
    u_int triangles_number_;
//    gener_type generator_;
};

} // <--- namespace testing

#endif

