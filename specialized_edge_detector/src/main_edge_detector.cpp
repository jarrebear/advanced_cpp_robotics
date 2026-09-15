#include "specialized_edge_detector/magic_subscriber_image_edgedetector.hpp"

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"

using sensor_msgs::msg::Image;

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);

  auto node = std::make_shared<rclcpp::Node>("magic_pcl_susbcriber_main_node");

  std::string topic_name = "/camera/image_raw";

  MagicSubscriber magic_subscriber_object;

  magic_subscriber_object.init<Image>(node, topic_name);

  rclcpp::spin(node);

  rclcpp::shutdown();
  return 0;
}
