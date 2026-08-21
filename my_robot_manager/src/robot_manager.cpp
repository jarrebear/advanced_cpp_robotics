#include "../include/my_robot_manager/robot_manager.hpp"

RobotManager::RobotManager(const std::string topic, const std::string name,
                           const std::string model)
    : Node("robot_manager_node_" + name), odometry_topic(topic),
      robot_name(name), robot_model(model) {

  odom_subscriber = this->create_subscription<nav_msgs::msg::Odometry>(
      odometry_topic, 1000,
      std::bind(&RobotManager::odom_callback, this, std::placeholders::_1));
  robot_count++;
  RCLCPP_INFO(this->get_logger(), "Robot %i created.", robot_count);
}

void RobotManager::print_specifications() {

  RCLCPP_INFO(this->get_logger(), "Robot Name: %s", robot_name.c_str());
  RCLCPP_INFO(this->get_logger(), "Robot Model: %s", robot_model.c_str());
  RCLCPP_INFO(this->get_logger(), "Battery Capacity (kWh): %f",
              battery_capacity_kwh);
  RCLCPP_INFO(this->get_logger(), "Total Operation Hours: %i",
              total_operation_hours);
}

void RobotManager::odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg) {
  RCLCPP_INFO(this->get_logger(), "%s position (x,y): %lf , %lf",
              robot_name.c_str(), msg->pose.pose.position.x,
              msg->pose.pose.position.y);
}