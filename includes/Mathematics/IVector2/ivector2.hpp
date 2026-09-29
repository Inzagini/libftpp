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

#include "ivector2.tpp"
