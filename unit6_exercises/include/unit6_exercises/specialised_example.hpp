#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "sensor_msgs/msg/point_cloud2.hpp"

using sensor_msgs::msg::Image;
using sensor_msgs::msg::PointCloud2;
using std::string;

template <typename ROSMessageType> class MagicSubscriber {
public:
  MagicSubscriber(const rclcpp::Node::SharedPtr &node,
                  const string &subscriber_topic);

  ~MagicSubscriber();

  void CallbackToTopic(const typename ROSMessageType::SharedPtr msg);

private:
  rclcpp::Node::SharedPtr node_;
  string subscriber_topic_;

  typename rclcpp::Subscription<ROSMessageType>::SharedPtr subscriber_;
};

// Template Definitions

template <typename ROSMessageType>
MagicSubscriber<ROSMessageType>::MagicSubscriber(
    const rclcpp::Node::SharedPtr &node, const string &subscriber_topic)
    : node_(node), subscriber_topic_(subscriber_topic) {

  subscriber_ = node_->create_subscription<ROSMessageType>(
      subscriber_topic_, rclcpp::SensorDataQoS(),
      std::bind(&MagicSubscriber<ROSMessageType>::CallbackToTopic, this,
                std::placeholders::_1));
}

template <typename ROSMessageType>
MagicSubscriber<ROSMessageType>::~MagicSubscriber() {
  std::cout << "MagicSubscriber Destructor is called" << std::endl;
}

template <typename ROSMessageType>
void MagicSubscriber<ROSMessageType>::CallbackToTopic(
    const typename ROSMessageType::SharedPtr msg) {

  RCLCPP_INFO_STREAM(node_->get_logger(),
                     "GENERIC Template Callback message = " << msg.get());
}

// Image specialization

template <>
inline void
MagicSubscriber<Image>::CallbackToTopic(const Image::SharedPtr msg) {

  RCLCPP_INFO_STREAM(node_->get_logger(),
                     "Specialised IMAGE Template Callback Data[0]="
                         << static_cast<int>(msg->data[0]));
}

// PointCloud2 specialization

template <>
inline void MagicSubscriber<PointCloud2>::CallbackToTopic(
    const PointCloud2::SharedPtr msg) {

  RCLCPP_INFO_STREAM(node_->get_logger(),
                     "Specialised POINTCLOUD Template Callback Data[0]="
                         << static_cast<int>(msg->data[0]));
}
