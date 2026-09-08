#include <iostream>

int main() {

  float r = 4.23;
  int k = 2;
  char a = 'a';
  int sum = k + r;
  std::cout << "sum: " << sum << " Here r is typecasted to be an integer\n ";
  sum = sum + a;
  std::cout
      << "sum: " << sum
      << " Here a is typecasted to be an integer, it's ASCII value is taken\n ";

  return 0;
}
