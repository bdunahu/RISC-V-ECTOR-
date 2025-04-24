#include "vec.h"

template<typename T>
Vec<T> Vec<T>::operator+(const Vec<T>& other) const {
    if (data.size() != other.data.size()) throw std::invalid_argument("Size mismatch");
    Vec<T> result;
    for (size_t i = 0; i < data.size(); ++i)
        result.data.push_back(data[i] + other.data[i]);
    return result;
}

template<typename T>
Vec<T> Vec<T>::operator-(const Vec<T>& other) const {
    if (data.size() != other.data.size()) throw std::invalid_argument("Size mismatch");
    Vec<T> result;
    for (size_t i = 0; i < data.size(); ++i)
        result.data.push_back(data[i] - other.data[i]);
    return result;
}

template<typename T>
Vec<T> Vec<T>::operator*(const Vec<T>& other) const {
    if (data.size() != other.data.size()) throw std::invalid_argument("Size mismatch");
    Vec<T> result;
    for (size_t i = 0; i < data.size(); ++i)
        result.data.push_back(data[i] * other.data[i]);
    return result;
}

template<typename T>
Vec<T> Vec<T>::operator/(const Vec<T>& other) const {
    if (data.size() != other.data.size()) throw std::invalid_argument("Size mismatch");
    Vec<T> result;
    for (size_t i = 0; i < data.size(); ++i)
        result.data.push_back(other.data[i] == 0 ? 0 : data[i] / other.data[i]);
    return result;
}

template<typename T>
Vec<T> Vec<T>::operator%(const Vec<T>& other) const {
    if (data.size() != other.data.size()) throw std::invalid_argument("Size mismatch");
    Vec<T> result;
    for (size_t i = 0; i < data.size(); ++i)
        result.data.push_back(other.data[i] == 0 ? 0 : data[i] % other.data[i]);
    return result;
}

// Scalar versions
template<typename T>
Vec<T> Vec<T>::operator+(T scalar) const {
    Vec<T> result;
    for (const auto& val : data) result.data.push_back(val + scalar);
    return result;
}

template<typename T>
Vec<T> Vec<T>::operator-(T scalar) const {
    Vec<T> result;
    for (const auto& val : data) result.data.push_back(val - scalar);
    return result;
}

template<typename T>
Vec<T> Vec<T>::operator*(T scalar) const {
    Vec<T> result;
    for (const auto& val : data) result.data.push_back(val * scalar);
    return result;
}

template<typename T>
Vec<T> Vec<T>::operator/(T scalar) const {
    Vec<T> result;
    for (const auto& val : data) result.data.push_back(scalar == 0 ? 0 : val / scalar);
    return result;
}

template<typename T>
Vec<T> Vec<T>::operator%(T scalar) const {
    Vec<T> result;
    for (const auto& val : data) result.data.push_back(scalar == 0 ? 0 : val % scalar);
    return result;
}

// Scalar - Vector versions
template<typename T>
Vec<T> operator-(T scalar, const Vec<T>& vec) {
    Vec<T> result;
    for (size_t i = 0; i < vec.size(); ++i)
        result = result + (scalar - vec.data[i]);
    return result;
}

template<typename T>
Vec<T> operator/(T scalar, const Vec<T>& vec) {
    Vec<T> result;
    for (auto val : vec.data)
        result = result + (val == 0 ? 0 : scalar / val);
    return result;
}

template<typename T>
Vec<T> operator%(T scalar, const Vec<T>& vec) {
    Vec<T> result;
    for (auto val : vec.data)
        result = result + (val == 0 ? 0 : scalar % val);
    return result;
}

template<typename T>
void Vec<T>::print() const {
    for (auto val : data) std::cout << val << " ";
    std::cout << std::endl;
}
