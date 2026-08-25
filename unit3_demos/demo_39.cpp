#include <iostream>

using namespace std;

class MyClass {
private:
  int *value; // Dynamically allocated memory

public:
  // Constructor
  MyClass(int v) {
    value = new int(v); // Allocate memory dynamically
  }

  // That is how you define a Copy Constructor for class MyClass
  MyClass(const MyClass &obj) {
    value = new int(*(obj.value)); // Deep copy
    cout << "Copy constructor called!" << endl;
  }

  // Destructor
  ~MyClass() {
    delete value; // Free the dynamically allocated memory
  }

  // Method to display the value
  void display() { cout << "Value: " << *value << endl; }
};

int main() {
  MyClass obj1(10);    // Create an object
  MyClass obj2 = obj1; // Create another object as a copy of obj1

  obj1.display(); // Display value of obj1
  obj2.display(); // Display value of obj2

  return 0;
}