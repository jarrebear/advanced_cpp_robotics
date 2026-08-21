#include <iostream>

class Icetray {
public:
  static int refillCount;

  // Constructor definition
  Icetray(int crystals = 2) {
    std::cout << "Constructor called." << std::endl;
    std::cout << crystals << " crystals are ready" << std::endl;
    // Increase every time object is created
    refillCount++;
  }
  // static member function
  static int getCount() { return refillCount; }
};

// Initialize static member of class
int Icetray::refillCount = 0;

int main(void) {
  // Declare object 1
  Icetray Orange(10);
  // Declare object 2
  Icetray Lemon(20);

  // Print total number of objects.
  std::cout << "Total refill counts: " << Icetray::getCount() << std::endl;

  return 0;
}
