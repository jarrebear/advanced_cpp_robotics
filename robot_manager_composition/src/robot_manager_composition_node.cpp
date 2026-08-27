#include "robot_manager_composition/robot_manager_composition.hpp"

int main(int argc, char **argv) {

  rclcpp::init(argc, argv);
  auto node = std::make_shared<rclcpp::Node>("robot_manager_using_composition");
  ComputerUnit computer_unit1;
  RobotManagerComposition rmc(node, computer_unit1);
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
