#include "robot_manager_inheritance/mobile_robot_robot_manager.hpp"

RobotManagerMobile::RobotManagerMobile(const rclcpp::Node::SharedPtr &node)
    : RobotManagerBase(node) {}

void RobotManagerMobile::displayRobotDetails() {
  RCLCPP_INFO(node_ptr->get_logger(), "Robot Name: %s", robot_name.c_str());
  RCLCPP_INFO(node_ptr->get_logger(), "Robot Location: %s",
              robot_location.c_str());
  RCLCPP_INFO(node_ptr->get_logger(), "Battery Charge Level: %.2f",
              battery_charge_level);
  RCLCPP_INFO(node_ptr->get_logger(), "Type of Basttery: %s",
              type_of_batttery.c_str());
}