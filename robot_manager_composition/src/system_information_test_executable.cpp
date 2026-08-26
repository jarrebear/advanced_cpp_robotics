#include "robot_manager_composition/system_information.hpp"

#include "rclcpp/rclcpp.hpp"

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);

  ComputerUnit computer_unit1;

  computer_unit1.print_info();

  rclcpp::shutdown();

  return 0;
}