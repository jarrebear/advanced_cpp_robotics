#include "../include/my_robot_manager/robot_manager.hpp"

RobotManager::RobotManager(const std::string topic, const std::string name,
                           const std::string model)
    : Node("robot_manager_node_" + name),
      odometry_topic("/" + name + "/" + topic), robot_name(name),
      robot_model(model) {

  odom_subscriber = this->create_subscription<nav_msgs::msg::Odometry>(
      odometry_topic, 1000,
      std::bind(&RobotManager::odom_callback, this, std::placeholders::_1));

  position_service = this->create_service<std_srvs::srv::Trigger>(
      "/" + robot_name + "/log_current_position",
      std::bind(&RobotManager::position_callback, this, std::placeholders::_1,
                std::placeholders::_2));

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
  current_x_position = msg->pose.pose.position.x;
  current_y_position = msg->pose.pose.position.y;
}

void RobotManager::position_callback(
    const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
    std::shared_ptr<std_srvs::srv::Trigger::Response> response) {
  RCLCPP_INFO(this->get_logger(), "%s position (x,y): %lf , %lf",
              robot_name.c_str(), current_x_position, current_y_position);
  response->success = true;
  response->message = "Position logged";
  return;
}