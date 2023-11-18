#pragma once

#include <array>

#include "point.hpp"

namespace yLAB {

class vector_t final {
public:
    using value_type     = double;
    using array          = std::array<value_type, 3>;
    using size_type      = std::size_t;
    using iterator       = typename std::array<value_type, 3>::iterator;
    using const_iterator = typename std::array<value_type, 3>::const_iterator;

    vector_t(value_type coord1, value_type coord2, value_type coord3);
    vector_t(const point_t& pt1, const point_t& pt2);
    vector_t(value_type coord_for_all);
    vector_t();
    ~vector_t() = default;
    
    bool operator==(const vector_t& rhs) const noexcept;

    vector_t& operator*=(value_type coeff) noexcept;
    vector_t& operator/=(value_type coeff);
    vector_t operator/(value_type coeff) const;
    
    value_type& operator[](size_type index);
    value_type operator[](size_type index) const;

    value_type& at(size_type index);
    value_type at(size_type index) const;

    value_type get_module() const;
    
    size_type size() const noexcept;

    bool is_null() const;
    bool is_valid() const;
    
    array coords() const noexcept; // for structure binding
private:
    array coords_;
};

std::ostream& operator<<(std::ostream& os, const vector_t& rhs);

vector_t operator+(const vector_t& lhs, const vector_t& rhs) noexcept;
vector_t operator-(const vector_t& lhs, const vector_t& rhs) noexcept;
vector_t operator*(const vector_t& vec, vector_t::value_type coeff) noexcept;
vector_t operator*(vector_t::value_type coeff, const vector_t& vec)  noexcept;

} // <--- namespace yLAB

