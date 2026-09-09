#ifndef LASER_READER_HPP
#define LASER_READER_HPP

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"

class LaserReaderClass : public rclcpp::Node {
private:
  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr laser_sub_;

  void laser_callback(const sensor_msgs::msg::LaserScan::SharedPtr laser_msg);

  
  float *last_laser_ranges;
  /* DEFINE THE POINTER TO THE LASER RANGES ARRAY HERE */

public:
  LaserReaderClass();
  ~LaserReaderClass();
};

#endif
