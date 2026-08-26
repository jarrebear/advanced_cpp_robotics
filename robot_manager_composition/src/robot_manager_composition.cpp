#include "robot_manager_composition/robot_manager_composition.hpp"

#include <functional>

RobotManagerComposition::RobotManagerComposition(
    const rclcpp::Node::SharedPtr &node)
    : node_ptr(node) {
  init_config_output_srv();
  RCLCPP_INFO(node_ptr->get_logger(),
              "Node created, /robot_manager_output service may be called.");
}

void RobotManagerComposition::init_config_output_srv() {
  output_service = node_ptr->create_service<std_srvs::srv::SetBool>(
      "robot_manager_output",
      std::bind(&RobotManagerComposition::output_callback, this,
                std::placeholders::_1, std::placeholders::_2));
}

void RobotManagerComposition::output_callback(
    const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
    std::shared_ptr<std_srvs::srv::SetBool::Response> response) {
  output_enabled = request->data;
  response->success = true;
  if (output_enabled) {
    response->message = "Console output enabled";
  } else {
    response->message = "Console output disabled";
  }
}