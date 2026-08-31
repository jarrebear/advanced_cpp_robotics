#include "robot_manager_inheritance/base_robot_manager.hpp"

int main(int argc, char **argv) {

  rclcpp::init(argc, argv);
  auto node = std::make_shared<rclcpp::Node>("robot_manager_base");
  RobotManagerBase rmb(node);
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}