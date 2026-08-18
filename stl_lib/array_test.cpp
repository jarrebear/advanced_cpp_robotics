#include <iostream>
#include <ostream>

int main() {
  int arr[2] = {42, 100};

  std::cout << arr[1] << " ";
  arr[1] = 80;
  std::cout << arr[1] << " ";
  std::cout << std::endl;

  return 0;
}
