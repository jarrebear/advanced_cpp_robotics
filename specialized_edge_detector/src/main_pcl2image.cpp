#include "specialized_edge_detector/magic_subscriber_pcl2image.hpp"

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>

using sensor_msgs::msg::PointCloud2;

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);

  auto node = std::make_shared<rclcpp::Node>("main_pcl2image_node",
                                             "main_pcl2image_ns");

  std::string topic_name = "/robot_camera/lidar";

  MagicSubscriber magic_subscriber_object;

  magic_subscriber_object.init<PointCloud2>(node, topic_name);

  rclcpp::spin(node);

  rclcpp::shutdown();
  return 0;
}
