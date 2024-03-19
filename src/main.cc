#include <iostream>

#include "header/fixed_point.h"

int main() {
  FixedPoint a(0.125f);
  FixedPoint b(0.375f);

  auto c = a / b;
  std::cout << c.GetFloat();

  return 0;
}
