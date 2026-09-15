#include "specialized_edge_detector/specialised_complete.hpp"

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "sensor_msgs/msg/point_cloud2.hpp"
#include "std_msgs/msg/string.hpp"

using sensor_msgs::msg::Image;
using sensor_msgs::msg::PointCloud2;

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);

  auto node = rclcpp::Node::make_shared("magic_subscriber_main_node");

  std::string topic_name_image = "/camera/image_raw";
  std::string topic_name_pointcloud = "/robot_camera/lidar";
  std::string topic_name = "/dummy_string";

  std::string publisher_topic_name_image = "/out/edge_detection";
  std::string publisher_topic_name_pointcloud = "/out/depth_image";

  MagicSubscriber<Image> magic_sub_obj_image(node, topic_name_image,
                                             publisher_topic_name_image);
  MagicSubscriber<PointCloud2> magic_sub_obj_pcl(
      node, topic_name_pointcloud, publisher_topic_name_pointcloud);
  MagicSubscriber<std_msgs::msg::String> magic_sub_obj_generic(node,
                                                               topic_name);

  rclcpp::spin(node);

  rclcpp::shutdown();
  return 0;
}
