#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>

#include "rclcpp/rclcpp.hpp"

template <typename MyClassTemplateArg> class MyClass {
public:
  void PrintInputType(const std::string &in_arg) {
    std::cout << "Print Type: " << typeid(in_arg).name() << std::endl;
  }
};

template <> class MyClass<std::string> {
public:
  void PrintInputString(const std::string &in_arg) {
    std::cout << "Message = " << in_arg << std::endl;
  }
};

template <typename DummyArgTemp> void Dummy_function(DummyArgTemp in_arg) {
  (void)in_arg; // Prevent unused parameter warning
  std::cout << "Dummy =" << std::endl;
}

template <> void Dummy_function<float>(float in_arg) {
  std::cout << in_arg << std::endl;
}

template <> void Dummy_function<std::string>(std::string in_arg) {
  std::cout << "Dummy for strings: " << in_arg << std::endl;
}

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);

  auto node = rclcpp::Node::make_shared(
      "basic_example_u6_specialization_functions_node");

  // Suppress unused variable warning.
  (void)node;

  MyClass<std::string> object_1;
  object_1.PrintInputString("Adventure Time!");

  MyClass<int> object_2;
  object_2.PrintInputType("Adventure Time!");

  int dummy_value = 0;
  Dummy_function<int>(dummy_value);

  float dummy_value_f = 3.14159265359f;
  Dummy_function<float>(dummy_value_f);

  std::string dummy_value_s = "Adventure Time!";
  Dummy_function(dummy_value_s);

  rclcpp::shutdown();

  return 0;
}
