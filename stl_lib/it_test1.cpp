#include <iostream>
#include <vector>

using namespace std;

int main() {
  vector<int> v{1, 2, 3, 4, 4, 1, 6, 7};
  vector<int>::iterator i;

  for (i = v.begin(); i != v.end(); i++) {
    cout << *i << "  ";
    // to get the value of the element pointed by iterator we use *
  }
  cout << "\n";

  return 0;
}
