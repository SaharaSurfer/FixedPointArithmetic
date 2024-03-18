#include "header/fixed_point.h"

#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <limits>

FixedPoint::FixedPoint(float value) {
  value_ = static_cast<int32_t>(value * (1 << kFractionBits));
}

FixedPoint::FixedPoint(int32_t value) {
  value_ = value;
}

float FixedPoint::GetFloat() {
  return static_cast<float>(value_) / (1 << kFractionBits);
}

FixedPoint FixedPoint::operator+(FixedPoint other) {
  int64_t result = value_ + other.value_;

  CheckOverUnderFlow(result);

  return FixedPoint(static_cast<int32_t>(result));
}

FixedPoint FixedPoint::operator-() {
  return FixedPoint(-value_);
}

FixedPoint FixedPoint::operator-(FixedPoint other) {
  return *this + (-other);
}

FixedPoint FixedPoint::operator*(FixedPoint other) {
  int64_t result = (value_ * other.value_) >> kFractionBits;

  CheckOverUnderFlow(result);

  return FixedPoint(static_cast<int32_t>(result));
}

FixedPoint FixedPoint::operator/(FixedPoint other) {
  int64_t result = static_cast<int64_t>(
    (static_cast<float>(value_) / other.value_) * (1 << kFractionBits));

  CheckOverUnderFlow(result);

  return FixedPoint(static_cast<int32_t>(result));
}

void FixedPoint::CheckOverUnderFlow(int64_t value) {
  if (value > std::numeric_limits<int32_t>::max()) {
    throw std::overflow_error("The maximum value in a fixed-point number exceeded!");
  }

  if (value < std::numeric_limits<int32_t>::min()) {
    throw std::underflow_error("The minimum value in a fixed-point number exceeded!");
  }
}
