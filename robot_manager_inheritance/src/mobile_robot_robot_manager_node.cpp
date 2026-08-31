#include "robot_manager_inheritance/mobile_robot_robot_manager.hpp"

int main(int argc, char **argv) {

  rclcpp::init(argc, argv);
  auto node = std::make_shared<rclcpp::Node>("robot_manager_mobile");
  RobotManagerMobile rmm(node);
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}