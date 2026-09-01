#include "robot_manager_inheritance/base_robot_manager.hpp"

RobotManagerBase::RobotManagerBase(const rclcpp::Node::SharedPtr &node)
    : node_ptr(node) {
  init_config_output_srv();
  RCLCPP_INFO(node_ptr->get_logger(),
              "Node created, /robot_manager_output service may be called.");
}

void RobotManagerBase::init_config_output_srv() {
  config_output_srv = node_ptr->create_service<std_srvs::srv::SetBool>(
      "robot_manager_output",
      std::bind(&RobotManagerBase::ConfigOutputCallback, this,
                std::placeholders::_1, std::placeholders::_2));
}

void RobotManagerBase::ConfigOutputCallback(
    const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
    std::shared_ptr<std_srvs::srv::SetBool::Response> response) {
  bool requested_output = request->data;

  displayRobotDetails();

  // Check if the requested configuration matches the current configuration
  if (requested_output == output_enabled) {
    response->success = false;
    response->message = "Output configuration request is the same as the "
                        "current output configuration.";
    return;
  }

  output_enabled = requested_output;
  response->success = true;

  if (!output_enabled) {
    response->message = "Console output disabled.";

    RCLCPP_INFO(node_ptr->get_logger(),
                "Robot Manager console output disabled.");
  } else {
    response->message = "Console output enabled.";

    RCLCPP_INFO(node_ptr->get_logger(),
                "Robot Manager console output enabled.");
  }
}

// void RobotManagerBase::displayRobotDetails() {
//   RCLCPP_INFO(node_ptr->get_logger(), "Robot Name: %s", robot_name.c_str());
//   RCLCPP_INFO(node_ptr->get_logger(), "Robot Location: %s",
//               robot_location.c_str());
// }