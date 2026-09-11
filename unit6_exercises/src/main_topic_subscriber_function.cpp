
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"

class MagicSubscriber {
public:
  MagicSubscriber() {
    std::cout << "MagicSubscriber Constructor is called" << std::endl;
  }

  template <typename T>
  void init(const rclcpp::Node::SharedPtr &node,
            const std::string &subscriber_topic) {

    node_ = node;
    subscriber_topic_ = subscriber_topic;
    
    // ROS2 uses INFO logging by default.

    image_sub_ = node_->create_subscription<T>(
        subscriber_topic_, 1,
        std::bind(&MagicSubscriber::CallbackToTopic<T>, this,
                  std::placeholders::_1));
  }

  ~MagicSubscriber() {
    std::cout << "MagicSubscriber Destructor is called" << std::endl;
  }

  template <typename T> void CallbackToTopic(const typename T::SharedPtr msg) {
    // uint8_t is typically an alias of unsigned char,
    // so cast to int before printing.
    if (!msg->data.empty()) {
      RCLCPP_INFO_STREAM(
          node_->get_logger(),
          "Call Back Topic Image Data[0]=" << static_cast<int>(msg->data[0]));
    }
  }

private:
  rclcpp::Node::SharedPtr node_;

  std::string subscriber_topic_;

  rclcpp::SubscriptionBase::SharedPtr image_sub_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);

  // Create a node in the "magic_subscriber_main_ns" namespace
  auto node = std::make_shared<rclcpp::Node>("magic_subscriber_main_node",
                                             "magic_subscriber_main_ns");

  std::string topic_name = "/camera/image_raw";

  MagicSubscriber magic_subscriber_object;

  magic_subscriber_object.init<sensor_msgs::msg::Image>(node, topic_name);

  rclcpp::spin(node);

  rclcpp::shutdown();
  return 0;
}
