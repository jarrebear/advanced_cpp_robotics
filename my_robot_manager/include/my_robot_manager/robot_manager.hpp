#ifndef ROBOT_MANAGER_HPP
#define ROBOT_MANAGER_HPP

#include <nav_msgs/msg/odometry.hpp>
#include <rclcpp/rclcpp.hpp>

#include <string>

class RobotManager : public rclcpp::Node {
public:
  static int robot_count;
  RobotManager(const std::string topic, const std::string name,
               const std::string model);
  void print_specifications();
  void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg);

private:
  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_subscriber;
  std::string odometry_topic;

  std::string robot_name;
  std::string robot_model;
  float battery_capacity_kwh{10.0f};
  int total_operation_hours{5};
};

#endif