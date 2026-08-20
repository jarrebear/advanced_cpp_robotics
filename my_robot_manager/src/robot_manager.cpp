#include "../include/my_robot_manager/robot_manager.hpp"

void RobotManager::print_specifications() {

  auto logger = rclcpp::get_logger("robot_manager");

  RCLCPP_INFO(logger, "Robot Name: %s", robot_name.c_str());
  RCLCPP_INFO(logger, "Robot Model: %s", robot_model.c_str());
  RCLCPP_INFO(logger, "Battery Capacity (kWh): %f", battery_capacity_kwh);
  RCLCPP_INFO(logger, "Total Operation Hours: %i", total_operation_hours);
}