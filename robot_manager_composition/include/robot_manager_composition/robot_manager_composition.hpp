#ifndef ROBOT_MANAGER_COMPOSITION_HPP
#define ROBOT_MANAGER_COMPOSITION_HPP

#include <rclcpp/rclcpp.hpp>
#include <std_srvs/srv/set_bool.hpp>

#include <memory>

class RobotManagerComposition {
public:
  explicit RobotManagerComposition(const rclcpp::Node::SharedPtr &node);

protected:
  rclcpp::Node::SharedPtr node_ptr;
  bool output_enabled{false};
  void init_config_output_srv();

private:
  void output_callback(
      const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
      std::shared_ptr<std_srvs::srv::SetBool::Response> response);

  rclcpp::Service<std_srvs::srv::SetBool>::SharedPtr output_service;
};

#endif