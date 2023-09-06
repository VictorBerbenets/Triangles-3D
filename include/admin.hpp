#ifndef ADMIN_CLASS_
#define ADMIN_CLASS_

#include <istream>

#include "oct_tree.hpp"

namespace yLAB {

class admin {
    using size_type = std::size_t;
    using dataVal   = std::pair<triangle_t, size_type>; // saving triangle and his order number
    
    static constexpr size_type SET_POINTS_SIZE  = 9;
public:
    admin(std::istream& stream);
    ~admin() = default;

    void print_intersected_triangles() const;
private:
    spaceBreaking::OctTree oct_tree_;
};

}

#endif


