#ifndef ADMIN_CLASS_
#define ADMIN_CLASS_

#include <istream>

#include "oct_tree.hpp"

namespace yLAB {

class admin {    
public:
    using size_type = std::size_t;
    using data_type = spaceBreaking::OctTree::data_type;

    static constexpr size_type SET_POINTS_SIZE  = 9;

template<typename Container>
    admin(const Container& data, double space_limit);
template<typename T>
    admin(T&& pair_data);
    ~admin() = default;

    void print_intersected_triangles() const;
private:
    spaceBreaking::OctTree oct_tree_;
};

template<typename Container>
admin::admin(const Container& data, double space_limit):
            oct_tree_ {space_limit} {
    for (auto& val : data) {
        oct_tree_.insert_triangle(val);
    }
};

template<typename T>
admin::admin(T&& pair_data): oct_tree_{pair_data.second} {
    for (auto& val : pair_data.first) {
        oct_tree_.insert_triangle(val);
    }
}

}

#endif

