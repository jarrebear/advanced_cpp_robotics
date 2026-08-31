#ifndef MOBILE_ROBOT_MANAGER_INHERITANCE_HPP
#define MOBILE_ROBOT_MANAGER_INHERITANCE_HPP

#include "robot_manager_inheritance/base_robot_manager.hpp"

class RobotManagerMobile : public RobotManagerBase {
public:
  explicit RobotManagerMobile(const rclcpp::Node::SharedPtr &node);
};

#endif