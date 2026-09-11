#include "unit6_exercises/magic_printer.hpp"

#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);

  // Create a node in the "magic_print_ns" namespace
  auto node =
      std::make_shared<rclcpp::Node>("magic_print_main_node", "magic_print_ns");

  MagicPrinter magic_printer_object(node);

  int value_integer = 42;
  magic_printer_object.PrintGeneric<int>(value_integer);

  std::string value_string = "Adventure Time is Back!";
  magic_printer_object.PrintGeneric<std::string>(value_string);

  rclcpp::shutdown();
  return 0;
}
