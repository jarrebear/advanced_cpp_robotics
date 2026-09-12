#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>

#include "rclcpp/rclcpp.hpp"

template <class MyClassTemplateArg> class MyClass {
public:
  void PrintInputType(std::string in_arg) {
    std::cout << "Print Type: " << typeid(in_arg).name() << std::endl;
  }
};

template <> class MyClass<std::string> {
public:
  void PrintInputString(const std::string &in_arg) {
    std::cout << "Message = " << in_arg << std::endl;
  }
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);

  auto node = rclcpp::Node::make_shared("basic_example_u6_specialization_node");

  (void)node; // Prevent unused-variable warning

  MyClass<std::string> object_1;
  object_1.PrintInputString("Adventure Time!");

  MyClass<int> object_2;
  object_2.PrintInputType("Adventure Time!");

  rclcpp::shutdown();
  return 0;
}
