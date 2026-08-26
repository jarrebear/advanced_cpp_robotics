#include "robot_manager_composition/robot_manager_composition.hpp"

int main(int argc, char **argv) {

  rclcpp::init(argc, argv);
  auto node = std::make_shared<rclcpp::Node>("robot_manager_using_composition");
  RobotManagerComposition rmc(node);
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
