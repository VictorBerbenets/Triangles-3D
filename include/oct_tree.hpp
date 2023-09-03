#include <iostream>
#include <list>
#include <cmath>
#include <memory>

#include "triangle.hpp"

namespace spaceBreaking {

using namespace yLAB;

class BoundingCube {
    using size_type = std::size_t;

    static constexpr size_type MAX_SPACE_DEGREE = 64;
    static constexpr size_type SPACE_BASE       = 2;
    static constexpr double MAX_CUBE_SIDE       = std::pow(2, 64);
    static constexpr double MIN_CUBE_SIDE       = 1;

    void set_center() const;
protected:
    BoundingCube();
    BoundingCube(const point_t& center);
    ~BoundingCube() = default;

    is_inside(const point_t& pt) const;
    
private:
    point_t center_;
    double side_size_;
}; // <--- class BoandingCube

class Node final: protected BoundingCube {
    
    bool is_limit_reached() const noexcept;

    void insert(const triangle_t& tria) const;
    void construct_new_cubes() const;
private:
    std::list<triangle_t> data_;
    
    std::unique_ptr<Node[]> childs_;
    std::unique_ptr<Node> parent_;


}; // <--- class Node

class OctTree final {
    using value_type       = std::unique_ptr<Node>;
    using const_value_type = const value_type;
public:
    OctTree();
    ~OctTree() = default;

    void insert_triangle(const triangle_t& tria) const;
    const_value_type get_root_node() const noexcept;
private:
    value_type root_node_;

};

} // <--- namespace spaceBreaking
