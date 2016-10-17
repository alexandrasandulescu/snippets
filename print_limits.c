#include <limits>
#include <iostream>

int main() {
  std::cout << "Minimum value for char: " << (int)std::numeric_limits<unsigned char>::min() << '\n';
  std::cout << "Maximum value for char: " << (int)std::numeric_limits<unsigned char>::max() << '\n';
}
