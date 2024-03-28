#include <cmath>
#include <iostream>

#include "header/fixed_point.h"

int main() {
  FixedPoint a(1.75);
  FixedPoint b(6.25f);

  auto c = a / b;
  std::cout << c.GetFloat() << std::endl;

  FixedPoint<24> d(std::sqrt(2));
  std::cout << d.GetFloat() << std::endl;

  FixedPoint<8> e((1 << 23) - 1);
  std::cout << e.GetFloat() << std::endl;
  
  return 0;
}
