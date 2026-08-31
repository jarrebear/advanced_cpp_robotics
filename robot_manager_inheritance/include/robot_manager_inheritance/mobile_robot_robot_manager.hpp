#ifndef MOBILE_ROBOT_MANAGER_INHERITANCE_HPP
#define MOBILE_ROBOT_MANAGER_INHERITANCE_HPP

#include "robot_manager_inheritance/base_robot_manager.hpp"

class RobotManagerMobile : public RobotManagerBase {
public:
  explicit RobotManagerMobile(const rclcpp::Node::SharedPtr &node);
  void displayRobotDetails();

private:
  float battery_charge_level{0.55f};
  std::string type_of_batttery{"Lead acid batteries"};
};

#endif