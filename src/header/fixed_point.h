#ifndef FIXEDPOINTARITHMETIC_HEADER_FIXED_POINT_H_
#define FIXEDPOINTARITHMETIC_HEADER_FIXED_POINT_H_

#include <cstdint>
#include <cmath>
#include <stdexcept>
#include <type_traits>

template<uint8_t fraction_bits=16>
class FixedPoint {
 public:
  template<typename Integral,
           typename = std::enable_if_t<std::is_integral_v<Integral>>>
  explicit FixedPoint(Integral value) {
    if (abs(value) > (1 << (31 - fraction_bits)) - 1) {
      throw std::overflow_error("Overflow in FixedPoint arithmetic.");
    }

    int64_t result = static_cast<int64_t>(value) << fraction_bits;
    CheckOverUnderFlow(result);
    value_ = static_cast<int32_t>(result);
  }

  template<typename Fraction,
           typename = std::enable_if_t<std::is_floating_point_v<Fraction>>,
           bool plug=true>
  explicit FixedPoint(Fraction value) {
    if (value > std::numeric_limits<double>::max() / (1 << fraction_bits)) {
        throw std::overflow_error("Overflow in FixedPoint arithmetic.");
    } else if (value < std::numeric_limits<double>::min() / (1 << fraction_bits)) {
        throw std::underflow_error("Underflow in FixedPoint arithmetic.");
    }

    int64_t result = static_cast<int64_t>(
        std::round(static_cast<double>(value) * (1 << fraction_bits)));
    CheckOverUnderFlow(result);
    value_ = static_cast<int32_t>(result);
  }

  float GetFloat() const {
    return static_cast<float>(value_) / (1 << fraction_bits);
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
    int64_t result = (static_cast<int64_t>(value_) + (other.GetUnderlyingInt() >> 1)) /
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
    if (value > std::numeric_limits<int32_t>::max()) {
      throw std::overflow_error("Overflow in FixedPoint arithmetic.");
    }

    if (value < std::numeric_limits<int32_t>::min()) {
      throw std::underflow_error("Underflow in FixedPoint arithmetic.");
    }
  }

  int32_t value_;
};

#endif  // FIXEDPOINTARITHMETIC_HEADER_FIXED_POINT_H_
