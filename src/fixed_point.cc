#include "header/fixed_point.h"

#include <cstdint>
#include <stdexcept>
#include <limits>

FixedPoint::FixedPoint(float value) {
  value_ = static_cast<int32_t>(value * (1 << fraction_bits_));
}

FixedPoint::FixedPoint(int32_t value) {
  value_ = value;
}

float FixedPoint::GetFloat() {
  return static_cast<float>(value_) / (1 << fraction_bits_);
}

FixedPoint FixedPoint::operator+(FixedPoint other) {
  int64_t result = value_ + other.value_;

  if (result > std::numeric_limits<int32_t>::max()) {
    throw std::overflow_error("The maximum value in a fixed-point number exceeded!");
  }

  if (result < std::numeric_limits<int32_t>::min()) {
    throw std::underflow_error("The minimum value in a fixed-point number exceeded!");
  }

  return FixedPoint(static_cast<int32_t>(result));
}

FixedPoint FixedPoint::operator-() {
  return FixedPoint(-value_);
}

FixedPoint FixedPoint::operator-(FixedPoint other) {
  return *this + (-other);
}
