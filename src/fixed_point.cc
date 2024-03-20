#include "header/fixed_point.h"

#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <limits>

FixedPoint::FixedPoint(float value) {
  value_ = static_cast<int32_t>(value * (1 << kFractionBits));
}

float FixedPoint::GetFloat() {
  return static_cast<float>(value_) / (1 << kFractionBits);
}

FixedPoint& FixedPoint::operator+=(const FixedPoint& other) {
  int64_t result = value_ + other.value_;

  CheckOverUnderFlow(result);

  value_ = static_cast<int32_t>(result);
  return *this;
}

FixedPoint& FixedPoint::operator-=(const FixedPoint& other) {
  int64_t result = value_ - other.value_;

  CheckOverUnderFlow(result);

  value_ = static_cast<int32_t>(result);
  return *this;
}

FixedPoint& FixedPoint::operator*=(const FixedPoint& other) {
  int64_t result = (value_ * other.value_) >> kFractionBits;

  CheckOverUnderFlow(result);

  value_ = static_cast<int32_t>(result);
  return *this;
}

FixedPoint& FixedPoint::operator/=(const FixedPoint& other) {
  int64_t result = static_cast<int64_t>(
    (static_cast<double>(value_) / other.value_) * (1 << kFractionBits));

  CheckOverUnderFlow(result);

  value_ = static_cast<int32_t>(result);
  return *this;
}

FixedPoint FixedPoint::operator+(const FixedPoint& other) const {
  FixedPoint result = *this;
  result += other;

  return result;
}

FixedPoint FixedPoint::operator-() const {
  FixedPoint result = *this;
  result.value_ = -result.value_;
  
  return result;
}

FixedPoint FixedPoint::operator-(const FixedPoint& other) const {
  FixedPoint result = *this;
  result -= other;

  return result;
}

FixedPoint FixedPoint::operator*(const FixedPoint& other) const {
  FixedPoint result = *this;
  result *= other;

  return result;
}

FixedPoint FixedPoint::operator/(const FixedPoint& other) const {
  FixedPoint result = *this;
  result /= other;

  return result;
}

void FixedPoint::CheckOverUnderFlow(int64_t value) const {
  if (value > std::numeric_limits<int32_t>::max()) {
    throw std::overflow_error("The maximum value in a fixed-point number exceeded!");
  }

  if (value < std::numeric_limits<int32_t>::min()) {
    throw std::underflow_error("The minimum value in a fixed-point number exceeded!");
  }
}
