#ifndef VECTOR_
#define VECTOR_

#include <array>

#include "point.hpp"

namespace yLAB {

class vector_t final {
    using size_type      = std::size_t;
    using iterator       = typename std::array<double, 3>::iterator;
    using const_iterator = typename std::array<double, 3>::const_iterator;
    using value_type     = double;
public:
    vector_t(value_type coord1, value_type coord2, value_type coord3);
    vector_t(const point_t& pt1, const point_t& pt2);
    vector_t(value_type coord_for_all);
    vector_t();
    ~vector_t() = default;
    
    vector_t operator*(value_type coeff) const noexcept;
    vector_t operator/(value_type coeff) const;
    
    value_type& operator[](size_type index);
    value_type operator[](size_type index) const;

    value_type& at(size_type index);
    value_type at(size_type index) const;

    value_type get_module() const;
    
    size_type size() const noexcept;

    bool is_null() const;
    bool is_valid() const;

    void print() const noexcept;
private:
    std::array<value_type, 3> coords_;
};


} // <--- namespace yLAB

#endif
