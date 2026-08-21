#include "my_robot_manager/robot_manager.hpp"

int main(int argc, char **argv) {
  // initialize Ros2 comms
  rclcpp::init(argc, argv);

  std::shared_ptr<RobotManager> robot;
  robot =
      std::make_shared<RobotManager>("/fastbot_1/odom", "fastbot_1", "FastBot");
  robot->print_specifications();

  rclcpp::spin(robot);
}