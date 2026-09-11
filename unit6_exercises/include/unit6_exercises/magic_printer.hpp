#pragma once

#include <memory>

#include "rclcpp/rclcpp.hpp"

class MagicPrinter {
public:
  explicit MagicPrinter(const rclcpp::Node::SharedPtr &node);

  ~MagicPrinter();

  template <typename T> void PrintGeneric(const T &in_value) {
    RCLCPP_INFO_STREAM(node_->get_logger(), "Generic Value=" << in_value);
  }

private:
  rclcpp::Node::SharedPtr node_;
};
