#ifndef ROBOT_MANAGER_HPP
#define ROBOT_MANAGER_HPP

#include <rclcpp/rclcpp.hpp>
#include <string>

class RobotManager {
public:
  std::string robot_name;
  std::string robot_model;
  float battery_capacity_kwh{10.0f};
  int total_operation_hours{5};

  void print_specifications();
};

#endif