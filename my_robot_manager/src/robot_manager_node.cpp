#include "my_robot_manager/robot_manager.hpp"

#include <iostream>

// Initialize static member of class
int RobotManager::robot_count = 0;

int main(int argc, char **argv) {
  // initialize Ros2 comms
  rclcpp::init(argc, argv);

  std::shared_ptr<RobotManager> robot_1;
  robot_1 = std::make_shared<RobotManager>("odom", "fastbot_1", "FastBot");
  robot_1->print_specifications();

  std::shared_ptr<RobotManager> robot_2;
  robot_2 = std::make_shared<RobotManager>("odom", "fastbot_2", "FastBot");
  robot_2->print_specifications();

  std::shared_ptr<RobotManager> robot_3;
  robot_3 = std::make_shared<RobotManager>("odom", "fastbot_3", "FastBot");
  robot_3->print_specifications();

  rclcpp::executors::SingleThreadedExecutor executor;

  executor.add_node(robot_1);
  executor.add_node(robot_2);
  executor.add_node(robot_3);

  executor.spin();
  rclcpp::shutdown();
  return 0;
}