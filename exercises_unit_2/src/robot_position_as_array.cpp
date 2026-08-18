#include <iostream>

#include "robot_commander/robot_commander.hpp"

#include <rclcpp/rclcpp.hpp>

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto my_robot = std::make_shared<RobotCommander>();

  float robot_position[3] = {my_robot->get_x_position(),
                             my_robot->get_y_position(),
                             my_robot->get_z_position()};

  for (float position : robot_position) {
    std::cout << position << " ";
  }

  std::cout << std::endl;
  rclcpp::shutdown();
  return 0;
}
