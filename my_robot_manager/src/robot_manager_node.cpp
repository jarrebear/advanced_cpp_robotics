#include "my_robot_manager/robot_manager.hpp"

int main(int argc, char **argv) {
  // initialize Ros2 comms
  rclcpp::init(argc, argv);

  RobotManager robot;
  robot.robot_name = "my_robot";
  robot.robot_model = "TurtleBot3";
  robot.print_specifications();
}