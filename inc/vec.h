// vec.h
#ifndef VEC_H
#define VEC_H

#include <iostream>
#include <vector>
#include <stdexcept>

template<typename T>
class Vec {
private:
    std::vector<T> data;

public:
    Vec() = default;
    Vec(const std::vector<T>& values) : data(values) {}

    // Vector-Vector Operations
    Vec<T> operator+(const Vec<T>& other) const;
    Vec<T> operator-(const Vec<T>& other) const;
    Vec<T> operator*(const Vec<T>& other) const;
    Vec<T> operator/(const Vec<T>& other) const;
    Vec<T> operator%(const Vec<T>& other) const;

    // Vector-Scalar Operations
    Vec<T> operator+(T scalar) const;
    Vec<T> operator-(T scalar) const;
    Vec<T> operator*(T scalar) const;
    Vec<T> operator/(T scalar) const;
    Vec<T> operator%(T scalar) const;

    // Utility
    void print() const;
    size_t size() const { return data.size(); }

    // Friend scalar-vector operations
    friend Vec<T> operator+(T scalar, const Vec<T>& vec) { return vec + scalar; }
    friend Vec<T> operator-(T scalar, const Vec<T>& vec);
    friend Vec<T> operator*(T scalar, const Vec<T>& vec) { return vec * scalar; }
    friend Vec<T> operator/(T scalar, const Vec<T>& vec);
    friend Vec<T> operator%(T scalar, const Vec<T>& vec);
};

#endif
