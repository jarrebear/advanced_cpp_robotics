#include <deque>
#include <iostream>

#include "robot_commander/robot_commander.hpp"

#include <rclcpp/rclcpp.hpp>

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto my_robot = std::make_shared<RobotCommander>();

  my_robot->move_in_circles();

  int total_values;
  std::cout << "Enter the number of heading values that you want to capture: ";

  // get user input from keyboard
  std::cin >> total_values;

  // vector we are using to store heading data
  std::deque<float> heading_data;
  for (int i = 0; i < total_values; ++i) {

    // Get current heading, save to vector and print to console
    float current_heading = my_robot->get_heading();
    heading_data.push_front(current_heading);
    std::cout << "Collecting heading value: " << current_heading << "\n";

    // Sleep for one second
    rclcpp::sleep_for(std::chrono::seconds(1));
  }

  std::cout << "Displaying saved data\n";
  for (size_t i = 0; i < heading_data.size(); ++i) {
    std::cout << "Heading value " << i << ": " << heading_data.at(i) << "\n";
  }

  // stop the robot
  my_robot->stop_moving();
  rclcpp::shutdown();
  return 0;
}
