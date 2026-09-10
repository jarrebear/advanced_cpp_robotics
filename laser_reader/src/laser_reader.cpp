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
  //   last_laser_ranges = new float[50];
  for (size_t i = 0; i <= 9; i++) {
    last_ten_scans[i] = new float[50];
  }
}

// Destructor
/* CREATE THE CLASS DESTRUCTOR HERE */

LaserReaderClass::~LaserReaderClass() {
  // delete[] last_laser_ranges;
  for (size_t i = 0; i <= 9; i++) {
    delete[] last_ten_scans[i];
  }
}

void LaserReaderClass::laser_callback(
    const sensor_msgs::msg::LaserScan::SharedPtr laser_msg) {

  // Need to copy oldest scan to assign to scan 0
  float *oldest_scan_buffer = last_ten_scans[9];

  // shift scan i into i+1 starting at scan 8
  for (size_t i = 9; i > 0; i--) {
    last_ten_scans[i] = last_ten_scans[i - 1];
  }

  // re-assign pointer to previous scan 9 to scan 0
  last_ten_scans[0] = oldest_scan_buffer;

  // Assign new scan data to our most recent scan (0 is always newest data, 9
  // oldest)

  for (size_t i = 0; i < laser_msg->ranges.size() && i < 50; i++) {
    last_ten_scans[0][i] = laser_msg->ranges[i];
  }

  //   std::cout << "Number of ranges: " << laser_msg->ranges.size() << "\n";
  if (scan_count_ >= 10) {

    std::cout
        << "Range measured by the ray at the front for the last 10 scans: \n";

    for (size_t i = 0; i <= 9; i++)
      std::cout << "Scan " << i << ": " << last_ten_scans[i][25] << "\n";
  }

  scan_count_++;
}

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);

  auto lrc = std::make_shared<LaserReaderClass>();

  rclcpp::spin(lrc);

  rclcpp::shutdown();
  return 0;
}
