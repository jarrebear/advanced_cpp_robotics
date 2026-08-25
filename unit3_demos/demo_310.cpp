#include <iostream>

using namespace std;

class Icetray {
  int crystal;
  int extra_trays;

public:
  Icetray() {
    crystal = 0;
    extra_trays = 0;
    string extra1 = "demo";
    float extra2 = 1.01;
  }
  // Constructor delegation
  Icetray(int crystal) : Icetray() {
    // avoid repetition
    this->crystal = crystal;
  }

  void show() { cout << extra_trays << '\t' << crystal << '\n'; }
};

int main() {
  Icetray Orange(3);
  Orange.show();
  return 0;
}