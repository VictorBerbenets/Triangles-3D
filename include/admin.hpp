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

template<typename Iter>
    admin(Iter begin, Iter end, double  space_limit);
    ~admin() = default;

    void print_intersected_triangles() const;
private:
    spaceBreaking::OctTree oct_tree_;
};

template<typename Iter>
admin::admin(Iter begin, Iter end, double  space_limit)
    : oct_tree_{space_limit} {
        for (; begin != end; ++begin) {
            oct_tree_.insert_triangle(*begin);
        }
    }
}

#endif

