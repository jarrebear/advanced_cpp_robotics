#include <chrono>
#include <memory>

#include "nav_msgs/msg/odometry.hpp"
#include "rclcpp/rclcpp.hpp"
#include "test_library/test_library.hpp"

using namespace std::chrono_literals;

class OdomListener : public rclcpp::Node {
public:
  OdomListener() : Node("odom_listener") {
    subscription_ = this->create_subscription<nav_msgs::msg::Odometry>(
        "/fastbot/odom", 1000,
        std::bind(&OdomListener::odomCallback, this, std::placeholders::_1));
  }

private:
  void odomCallback(const nav_msgs::msg::Odometry::SharedPtr msg) {
    float x = msg->pose.pose.position.x;
    float y = msg->pose.pose.position.y;

    rclcpp::sleep_for(1s);

    display_pos(x, y);

    float lin_vel = msg->twist.twist.linear.x;
    float ang_vel = msg->twist.twist.angular.z;

    rclcpp::sleep_for(1s);

    display_vel(lin_vel, ang_vel);
  }

  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr subscription_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);

  auto node = std::make_shared<OdomListener>();

  rclcpp::spin(node);

  rclcpp::shutdown();
  return 0;
}
