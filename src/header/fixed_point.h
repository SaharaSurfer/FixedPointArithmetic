#ifndef FIXEDPOINTARITHMETIC_HEADER_FIXED_POINT_H_
#define FIXEDPOINTARITHMETIC_HEADER_FIXED_POINT_H_

#include <cstdint>

class FixedPoint {
 public:
  FixedPoint(float value);
  FixedPoint(int32_t value);

  float GetFloat();

  FixedPoint operator+(FixedPoint other);

 private:
  static const uint8_t fraction_bits_ = 16;

  int32_t value_;
};

#endif  // FIXEDPOINTARITHMETIC_HEADER_FIXED_POINT_H_
