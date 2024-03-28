#ifndef FIXEDPOINTARITHMETIC_HEADER_FIXED_POINT_H_
#define FIXEDPOINTARITHMETIC_HEADER_FIXED_POINT_H_

#include <cstdint>
#include <cmath>
#include <stdexcept>
#include <type_traits>

template<uint8_t bits>
concept less_int_bits = bits < 32;

template<uint8_t fraction_bits=16> requires less_int_bits<fraction_bits>
class FixedPoint {
 public:
  template<typename NumericType>
  explicit FixedPoint(NumericType value) {
    int32_t max_int = (1LL << (31 - fraction_bits)) - 1;

    if (abs(value) > max_int) {
      throw std::overflow_error("Overflow in FixedPoint arithmetic.");
    }

    value_ = static_cast<int32_t>(std::round(value * (1 << fraction_bits)));
  }

  float GetFloat() const {
    return value_ / (1 << fraction_bits) +
        static_cast<float>(value_ % (1 << fraction_bits)) / (1 << fraction_bits);
  }

  int32_t GetUnderlyingInt() const {
    return value_;
  }

  template<uint8_t other_fb>
  FixedPoint& operator+=(const FixedPoint<other_fb>& other) {
    int64_t result = static_cast<int64_t>(value_) + GetScaledValue(other);

    CheckOverUnderFlow(result);

    value_ = static_cast<int32_t>(result);
    return *this;
  }

  template<uint8_t other_fb>
  FixedPoint& operator-=(const FixedPoint<other_fb>& other) {
    int64_t result = static_cast<int64_t>(value_) - GetScaledValue(other);

    CheckOverUnderFlow(result);

    value_ = static_cast<int32_t>(result);
    return *this;
  }

  template<uint8_t other_fb>
  FixedPoint& operator*=(const FixedPoint<other_fb>& other) {
    int64_t result = (static_cast<int64_t>(value_) * 
                      other.GetUnderlyingInt()) >> other_fb;

    CheckOverUnderFlow(result);

    value_ = static_cast<int32_t>(result);
    return *this;
  }

  template<uint8_t other_fb>
  FixedPoint& operator/=(const FixedPoint<other_fb>& other) {
    int64_t result = (static_cast<int64_t>(value_) +
                      (other.GetUnderlyingInt() >> 1)) /
                     other.GetUnderlyingInt() << other_fb;

    CheckOverUnderFlow(result);

    value_ = static_cast<int32_t>(result);
    return *this;
  }

  template<uint8_t other_fb>
  FixedPoint operator+(const FixedPoint<other_fb>& other) const {
    FixedPoint<fraction_bits> result = *this;
    result += other;

    return result;
  }

  FixedPoint operator-() const {
    FixedPoint<fraction_bits> result = *this;
    result.value_ = -result.value_;
    
    return result;
  }

  template<uint8_t other_fb>
  FixedPoint operator-(const FixedPoint<other_fb>& other) const {
    FixedPoint<fraction_bits> result = *this;
    result -= other;

    return result;
  }

  template<uint8_t other_fb>
  FixedPoint operator*(const FixedPoint<other_fb>& other) const {
    FixedPoint<fraction_bits> result = *this;
    result *= other;

    return result;
  }

  template<uint8_t other_fb>
  FixedPoint operator/(const FixedPoint<other_fb>& other) const {
    FixedPoint<fraction_bits> result = *this;
    result /= other;

    return result;
  }

 private:
  template<uint8_t other_fb>
  constexpr int64_t GetScaledValue(const FixedPoint<other_fb>& other) const {
    int64_t result = other.GetUnderlyingInt();

    if constexpr (fraction_bits > other_fb) {
      result <<= (fraction_bits - other_fb);
    } else if constexpr (fraction_bits < other_fb) {
      result >>= abs(fraction_bits - other_fb);
    }

    CheckOverUnderFlow(result);

    return result;
  }

  constexpr void CheckOverUnderFlow(int64_t value) const {
    if (value > static_cast<int64_t>(std::numeric_limits<int32_t>::max())) {
      throw std::overflow_error("Overflow in FixedPoint arithmetic.");
    }

    if (value < static_cast<int64_t>(std::numeric_limits<int32_t>::min())) {
      throw std::underflow_error("Underflow in FixedPoint arithmetic.");
    }
  }

  int32_t value_;
};

#endif  // FIXEDPOINTARITHMETIC_HEADER_FIXED_POINT_H_
