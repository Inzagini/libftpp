#pragma once

#include <cmath>
#include <stdexcept>
template <typename T> struct IVector3 {

  T x{};
  T y{};
  T z{};

  constexpr IVector3() = default;
  constexpr IVector3(const T& x, const T& y, const T& z);

  constexpr IVector3<T> operator+(const IVector3<T>& other) const;
  constexpr IVector3<T> operator-(const IVector3<T>& other) const;
  constexpr IVector3<T> operator*(const IVector3<T>& other) const;
  constexpr IVector3<T> operator*(const T& scalar) const;
  constexpr IVector3<T> operator/(const IVector3<T>& other) const;
  constexpr IVector3<T> operator/(const T& scalar) const;

  constexpr bool operator==(const IVector3<T>& other) const;
  constexpr bool operator!=(const IVector3<T>& other) const;

  float length() const;

  IVector3<float> normalize() const;
  T dot(const IVector3<T>& other) const;
  IVector3<T> cross(const IVector3<T>& other) const;
};

#include "ivector3.tpp"
