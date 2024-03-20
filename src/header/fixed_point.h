#ifndef FIXEDPOINTARITHMETIC_HEADER_FIXED_POINT_H_
#define FIXEDPOINTARITHMETIC_HEADER_FIXED_POINT_H_

#include <cstdint>

class FixedPoint {
 public:
  FixedPoint(float value);

  float GetFloat();

  FixedPoint& operator+=(const FixedPoint& other);
  FixedPoint& operator-=(const FixedPoint& other);
  FixedPoint& operator*=(const FixedPoint& other);
  FixedPoint& operator/=(const FixedPoint& other);

  FixedPoint operator+(const FixedPoint& other) const;
  FixedPoint operator-() const;
  FixedPoint operator-(const FixedPoint& other) const;
  FixedPoint operator*(const FixedPoint& other) const;
  FixedPoint operator/(const FixedPoint& other) const;

 private:
  static const uint8_t kFractionBits = 16;

  void CheckOverUnderFlow(int64_t value) const;

  int32_t value_;
};

#endif  // FIXEDPOINTARITHMETIC_HEADER_FIXED_POINT_H_
