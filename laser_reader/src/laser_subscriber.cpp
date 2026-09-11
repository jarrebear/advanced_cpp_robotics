#include <cmath>
#include <limits>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"

class LaserSubscriberNode : public rclcpp::Node {
public:
  LaserSubscriberNode() : Node("laser_subscriber_node") {
    laser_scan_sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
        "/fastbot_1/scan", 10,
        std::bind(&LaserSubscriberNode::laserScanCallback, this,
                  std::placeholders::_1));
  }

private:
  void laserScanCallback(const sensor_msgs::msg::LaserScan::SharedPtr msg) {
    // Store the received message
    laser_scan_ = msg;

    // Print the range at index 25 (robot front)
    if (!laser_scan_->ranges.empty()) {
      float range = laser_scan_->ranges[25];

      //   RCLCPP_INFO(this->get_logger(), "Range: %f", range);
    }

    printClosestObstacleDistance(msg);
  }

  void printClosestObstacleDistance(
      const sensor_msgs::msg::LaserScan::SharedPtr msg) {
    float closest_dist = std::numeric_limits<float>::infinity();
    float angle = std::numeric_limits<float>::infinity();

    for (size_t i = 0; i < msg->ranges.size(); i++) {
      if (closest_dist > msg->ranges[i]) {
        closest_dist = msg->ranges[i];
        angle = msg->angle_min + msg->angle_increment * i;
      }
    }

    std::cout << "Closest obstacle distance: " << closest_dist
              << ", Angle (rad): " << angle << "\n";
  }

  // Store the received LaserScan message
  sensor_msgs::msg::LaserScan::SharedPtr laser_scan_;

  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr laser_scan_sub_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);

  auto node = std::make_shared<LaserSubscriberNode>();

  rclcpp::spin(node);

  rclcpp::shutdown();
  return 0;
}
