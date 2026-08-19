#include <iostream>
#include <set>

#include "robot_commander/robot_commander.hpp"

#include <rclcpp/rclcpp.hpp>

std::set<std::pair<float, float>>
store_points(std::set<std::pair<float, float>> current_set,
             std::shared_ptr<RobotCommander> robot_commander) {
  float x_pos = robot_commander->get_x_position();
  float y_pos = robot_commander->get_y_position();

  current_set.insert({x_pos, y_pos});
  return current_set;
}

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto my_robot = std::make_shared<RobotCommander>();

  std::set<std::pair<float, float>> polygon_vertices;

  // Start point
  polygon_vertices = store_points(polygon_vertices, my_robot);

  // Point 1, moving forward
  my_robot->move_forward(1);
  polygon_vertices = store_points(polygon_vertices, my_robot);

  // Point 2, turn then forward
  my_robot->turn(0.785, 2);
  my_robot->move_forward(1);
  polygon_vertices = store_points(polygon_vertices, my_robot);

  // printing set of vertices
  std::set<std::pair<float, float>>::iterator itr;
  std::cout << "\nThe element of Vertex are : \n";
  for (itr = polygon_vertices.begin(); itr != polygon_vertices.end(); itr++) {
    std::cout << "(" << itr->first << ", " << itr->second << ") \n";
  }

  rclcpp::shutdown();
  return 0;
}
