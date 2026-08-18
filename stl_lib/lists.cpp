#include <iostream>
#include <iterator>
#include <list>

using namespace std;

int main() {
  list<string> laser{"range_max", "range_min", "angle_max", "angle_min"};

  auto it = laser.begin();

  cout << "The first element : ";
  cout << *it << "\n";

  advance(it, 2);
  cout << "The third element : ";
  cout << *it << "\n";

  it = laser.end();

  advance(it, -3);
  cout << "The third element from the back : ";
  cout << *it << "\n";

  return 0;
}
