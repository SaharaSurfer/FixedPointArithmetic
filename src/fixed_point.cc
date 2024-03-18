#include "header/fixed_point.h"

FixedPoint::FixedPoint(float value) {
  value_ = static_cast<int32_t>(value * (1 << fraction_bits_));
}

float FixedPoint::GetFloat() {
  return static_cast<float>(value_) / (1 << fraction_bits_);
}