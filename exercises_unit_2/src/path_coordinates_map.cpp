#include <iostream>
#include <map>

#include "robot_commander/robot_commander.hpp"

#include <rclcpp/rclcpp.hpp>

// map<int, std::pair<float, float>>

std::map<int, std::pair<float, float>>
store_points(std::map<int, std::pair<float, float>> current_map,
             std::shared_ptr<RobotCommander> robot_commander, int key) {
  float x_pos = robot_commander->get_x_position();
  float y_pos = robot_commander->get_y_position();

  current_map[key] = {x_pos, y_pos};
  return current_map;
}

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto my_robot = std::make_shared<RobotCommander>();

  std::map<int, std::pair<float, float>> polygon_vertices;

  // Start point
  polygon_vertices = store_points(polygon_vertices, my_robot, 1);

  // Point 1, moving forward
  my_robot->move_forward(1);
  polygon_vertices = store_points(polygon_vertices, my_robot, 2);

  // Point 2, turn then forward
  my_robot->turn(0.785, 2);
  my_robot->move_forward(1);
  polygon_vertices = store_points(polygon_vertices, my_robot, 3);

  // printing set of vertices
  std::cout << "Vertcies formed by robot has follwoing coordinates mapped to "
               "numbering: \n";
  std::map<int, std::pair<float, float>>::iterator itr;
  for (itr = polygon_vertices.begin(); itr != polygon_vertices.end(); ++itr) {
    std::cout << '\t' << "(" << itr->second.first << ", " << '\t'
              << itr->second.second << ") -> " << itr->first << "\n";
  }

  rclcpp::shutdown();
  return 0;
}
