#ifndef ADMIN_CLASS_
#define ADMIN_CLASS_

#include <istream>

#include "oct_tree.hpp"

namespace yLAB {

class admin {    
public:
    using size_type = std::size_t;
    using dataVal   = std::pair<triangle_t, size_type>; // saving triangle and his order number

    static constexpr size_type SET_POINTS_SIZE  = 9;

template<typename Container>
    admin(const Container& data, double space_limit);
template<typename T>
    admin(T&& pair_data);
/*template<typename Container>
admin::admin(std::pair<Container, double>&& pair_data): 
            admin{pair_data.first, pair_data.second} {}*/
/*
template<typename Container>
    admin(const Container& data, double space_limit);
template<typename Container>
    admin(const std::pair<Container, double>& pair_data);*/
/*template<typename Container>
admin::admin(std::pair<Container, double>&& pair_data): 
            admin{pair_data.first, pair_data.second} {}*/
//    admin(std::istream& stream);
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
           // admin{std::forward<>pair_data.first, pair_data.second} {

    for (auto& val : pair_data.first) {
        oct_tree_.insert_triangle(val);
    }
}

/*
template<typename Container>
admin::admin(std::pair<Container, double>&& pair_data): 
            admin{pair_data.first, pair_data.second} {
    std::cout << "RRRRRVALUEEEEEEEEEEEEEEEEEEEE\n";
}*/

}

#endif

