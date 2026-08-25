#ifndef ROBOT_MANAGER_HPP
#define ROBOT_MANAGER_HPP

#include <nav_msgs/msg/odometry.hpp>
#include <rclcpp/rclcpp.hpp>
#include <std_srvs/srv/trigger.hpp>

#include <string>

class RobotManager : public rclcpp::Node {
public:
  static int robot_count;
  RobotManager(const std::string topic, const std::string name,
               const std::string model);
  void print_specifications();
  void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg);
  void position_callback(
      const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
      std::shared_ptr<std_srvs::srv::Trigger::Response> response);

private:
  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_subscriber;
  rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr position_service;

  std::string odometry_topic;

  std::string robot_name;
  std::string robot_model;
  float battery_capacity_kwh{10.0f};
  int total_operation_hours{5};
  float current_x_position;
  float current_y_position;
};

#endif