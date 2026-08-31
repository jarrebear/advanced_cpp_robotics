#ifndef ROBOT_MANAGER_INHERITANCE_HPP
#define ROBOT_MANAGER_INHERITANCE_HPP

#include <rclcpp/rclcpp.hpp>
#include <std_srvs/srv/set_bool.hpp>

#include <memory>

class RobotManagerBase {
public:
  explicit RobotManagerBase(const rclcpp::Node::SharedPtr &node);

protected:
  rclcpp::Node::SharedPtr node_ptr;
  bool output_enabled{false};
  void init_config_output_srv();

private:
  void ConfigOutputCallback(
      const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
      std::shared_ptr<std_srvs::srv::SetBool::Response> response);

  rclcpp::Service<std_srvs::srv::SetBool>::SharedPtr config_output_srv;
};

#endif