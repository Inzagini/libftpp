#pragma once

#include "Mathematics/IVector3/ivector3.hpp"
#include <cmath>
#include <stdexcept>
template <typename T> struct IVector2 {

  T x{};
  T y{};

  constexpr IVector2() = default;
  constexpr IVector2(T x, T y);

  constexpr IVector2 operator+(const IVector2& other) const;
  constexpr IVector2 operator-(const IVector2& other) const;
  constexpr IVector2 operator*(const IVector2& other) const;
  constexpr IVector2 operator*(const T& scalar) const;
  constexpr IVector2 operator/(const IVector2& other) const;
  constexpr IVector2 operator/(const T& scalar) const;
  constexpr bool operator==(const IVector2& other) const;
  constexpr bool operator!=(const IVector2& other) const;

  float length() const;

  IVector2<float> normalize() const;

  T dot(const IVector2& other) const;
  T cross(const IVector2& other) const;
};

template <typename T> constexpr IVector2<T>::IVector2(T x, T y) : x(x), y(y) {}

template <typename T>
constexpr IVector2<T> IVector2<T>::operator+(const IVector2<T>& other) const {

  return {x + other.x, y + other.y};
}

template <typename T>
constexpr IVector2<T> IVector2<T>::operator-(const IVector2<T>& other) const {

  return {x - other.x, y - other.y};
}

template <typename T>
constexpr IVector2<T> IVector2<T>::operator*(const IVector2<T>& other) const {

  return {x * other.x, y * other.y};
}

template <typename T>
constexpr IVector2<T> IVector2<T>::operator*(const T& scalar) const {
  return {scalar * x, scalar * y};
}

template <typename T>
constexpr IVector2<T> operator*(const T& scalar, const IVector2<T>& other) {
  return {scalar * other.x, scalar * other.y};
}

template <typename T>
constexpr IVector2<T> IVector2<T>::operator/(const IVector2<T>& other) const {
  if (other.x == 0 || other.y)
    throw std::invalid_argument("Division by zero");
  return {x / other.x, y / other.y};
}

template <typename T>
constexpr IVector2<T> IVector2<T>::operator/(const T& scalar) const {
  if (scalar == 0)
    throw std::invalid_argument("Division by zero");
  return {x / scalar, y / scalar};
}

template <typename T>
constexpr IVector2<T> operator/(const T& scalar, const IVector2<T>& other) {
  if (other.x == 0 || other.y == 0 || other.z == 0)
    throw std::invalid_argument("Division by zero");
  return {scalar / other.x, scalar / other.y};
}

template <typename T> float IVector2<T>::length() const {
  return std::sqrt(static_cast<float>(x) * x + static_cast<float>(y) * y);
}

template <typename T> IVector2<float> IVector2<T>::normalize() const {
  float len = length();
  if (len == 0.0f)
    return {0.0, 0.0};
  return {static_cast<float>(x) / len, static_cast<float>(y) / len};
}

template <typename T> T IVector2<T>::dot(const IVector2<T>& other) const {
  return (x * other.x + y * other.y);
}

template <typename T> T IVector2<T>::cross(const IVector2<T>& other) const {
  return (x * other.y - y * other.x);
}

template <typename T>
constexpr bool IVector2<T>::operator==(const IVector2<T>& other) const {
  return (x == other.x && y == other.y);
}

template <typename T>
constexpr bool IVector2<T>::operator!=(const IVector2<T>& other) const {
  return (x != other.x || y != other.y);
}
