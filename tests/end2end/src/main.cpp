#include <iostream>

#include "end2end_generator.hpp"

int main(int argc, const char* argv[]) {
    using namespace testing;
    
    if (argc != 3) {
        throw std::invalid_argument{"error: expected two parameters: file name and number of triangles"};
    }
#if 0    
    std::size_t triangles_number = std::stoi(argv[2]);
    generator gen(argv[1], triangles_number);

    gen.generate_triangles();
#endif
}
