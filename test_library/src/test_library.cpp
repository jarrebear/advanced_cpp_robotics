#include "../include/test_library/test_library.hpp"

void display_pos(float x, float y) {

  float pos_x = x;
  float pos_y = y;
  std::cout << "Position X: " << pos_x << " Position Y: " << pos_y << std::endl;
}

void display_vel(float lin_vel, float ang_vel) {
  std::cout << "Linear velocity: " << lin_vel
            << " Angular velocity: " << ang_vel << std::endl;
}