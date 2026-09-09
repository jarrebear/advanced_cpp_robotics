#include "laser_reader/laser_reader.hpp"

#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

// Constructor
LaserReaderClass::LaserReaderClass() : Node("laser_reader") {
  laser_sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
      "/fastbot_1/scan", 10,
      std::bind(&LaserReaderClass::laser_callback, this,
                std::placeholders::_1));

  RCLCPP_INFO(this->get_logger(), "Initializing node.");

  rclcpp::sleep_for(2s);

  // Allocate an array of 50 floats
  last_laser_ranges = new float[50];
}

// Destructor
/* CREATE THE CLASS DESTRUCTOR HERE */

LaserReaderClass::~LaserReaderClass() { delete[] last_laser_ranges; }

void LaserReaderClass::laser_callback(
    const sensor_msgs::msg::LaserScan::SharedPtr laser_msg) {

  for (size_t i = 0; i < laser_msg->ranges.size() && i < 50; i++) {
    last_laser_ranges[i] = laser_msg->ranges[i];
  }
  std::cout << "Number of ranges: " << laser_msg->ranges.size() << "\n";

  std::cout << "Distance measured by the ray at the front of the robot: "
            << last_laser_ranges[25] << "\n";
}

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);

  auto lrc = std::make_shared<LaserReaderClass>();

  rclcpp::spin(lrc);

  rclcpp::shutdown();
  return 0;
}
