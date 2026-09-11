#include "unit6_exercises/magic_printer.hpp"

#include <iostream>

MagicPrinter::MagicPrinter(const rclcpp::Node::SharedPtr &node) : node_(node) {
  // ROS2 uses INFO as the default logging level, so no explicit
  // logger configuration is required.
}

MagicPrinter::~MagicPrinter() {
  std::cout << "MagicPrinter Destructor is called" << std::endl;
}
