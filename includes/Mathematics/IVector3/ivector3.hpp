#pragma once

#include <cmath>
template <typename T> struct IVector3 {

  T x{};
  T y{};
  T z{};

  constexpr IVector3<T> operator+(const IVector3<T>& other) const;
  constexpr IVector3<T> operator-(const IVector3<T>& other) const;
  constexpr IVector3<T> operator*(const IVector3<T>& other) const;
  constexpr IVector3<T> operator*(const T& scalar) const;
  constexpr IVector3<T> operator/(const IVector3<T>& other) const;

  constexpr bool operator==(const IVector3<T>& other) const;
  constexpr bool operator!=(const IVector3<T>& other) const;

  float length() const;

  IVector3<float> normalize() const;
  T dot(const IVector3<T>& other) const;
  T cross(const IVector3<T>& other) const;
};

template <typename T>
constexpr IVector3<T> IVector3<T>::operator+(const IVector3<T>& other) const {
  return {x + other.x, y + other.y, z + other.z};
};

template <typename T>
constexpr IVector3<T> IVector3<T>::operator-(const IVector3<T>& other) const {
  return {x - other.x, y - other.y, z - other.z};
};

template <typename T>
constexpr IVector3<T> IVector3<T>::operator*(const IVector3<T>& other) const {
  return {x * other.x, y * other.y, z * other.z};
};

template <typename T>
constexpr IVector3<T> IVector3<T>::operator*(const T& scalar) const {
  return {scalar * x, scalar * y, scalar * z};
};

template <typename T>
constexpr IVector3<T> IVector3<T>::operator/(const IVector3<T>& other) const {
  return {x / other.x, y / other.y, z / other.z};
};

template <typename T>
constexpr bool IVector3<T>::operator==(const IVector3<T>& other) const {
  return (x == other.x && y == other.y && z == other.z);
}

template <typename T>
constexpr bool IVector3<T>::operator!=(const IVector3<T>& other) const {
  return (x != other.x || y != other.y || z != other.z);
}

template <typename T> float IVector3<T>::length() const {
  return std::sqrt(static_cast<float>(x) * x + static_cast<float>(y) * y);
}

template <typename T> IVector3<float> IVector3<T>::normalize() const {
  float len = length();
  if (len == 0.0f)
    return {0.0, 0.0, 0.0};

  return {x / len, y / len, z / len};
}

template <typename T> T IVector3<T>::dot(const IVector3<T>& other) const {
  return {x * other.x, y * other.y, z * other.z};
}

template <typename T> T IVector3<T>::cross(const IVector3<T>& other) const {
  return {y * other.z - z * other.y, z * other.x - x * other.z,
          x * other.y - y * other.x};
}
