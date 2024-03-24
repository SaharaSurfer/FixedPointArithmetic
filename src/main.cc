#include <cmath>
#include <iostream>

#include "header/fixed_point.h"

int main() {
  FixedPoint<24> a(1.23f);
  FixedPoint<8> b(6.25f);

  auto c = a * b;
  std::cout << c.GetFloat() << std::endl;

  FixedPoint<24> d(std::sqrt(2));
  std::cout << d.GetFloat() << std::endl;

  FixedPoint<8> e(12345.6789f);
  std::cout << e.GetFloat() << std::endl;

  return 0;
}
