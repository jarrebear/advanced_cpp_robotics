cmake_minimum_required(VERSION 3.8)
project(robot_manager_inheritance)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(std_srvs REQUIRED)

# Include header files
include_directories(include)

##################################################
# Library
##################################################

add_library(base_robot_manager SHARED
  src/base_robot_manager.cpp
)

target_include_directories(base_robot_manager
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(base_robot_manager
  rclcpp
  std_srvs
)

add_library(mobile_robot_robot_manager SHARED
  src/mobile_robot_robot_manager.cpp
)

target_include_directories(mobile_robot_robot_manager
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(mobile_robot_robot_manager
  base_robot_manager
)

ament_target_dependencies(mobile_robot_robot_manager
  rclcpp
  std_srvs
  nav_msgs
)

##################################################
# Executable
##################################################

add_executable(base_robot_manager_node
  src/base_robot_manager_node.cpp
)

target_link_libraries(base_robot_manager_node
  base_robot_manager
)

ament_target_dependencies(base_robot_manager_node
  rclcpp
  std_srvs
)

install(
  TARGETS
	base_robot_manager
	base_robot_manager_node
  EXPORT export_base_robot_manager
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)


add_executable(mobile_robot_robot_manager_node
  src/mobile_robot_robot_manager_node.cpp
)

target_link_libraries(mobile_robot_robot_manager_node
  base_robot_manager
  mobile_robot_robot_manager
)

ament_target_dependencies(mobile_robot_robot_manager_node
  rclcpp
  std_srvs
  nav_msgs
)

install(
  TARGETS
	mobile_robot_robot_manager
	mobile_robot_robot_manager_node
  EXPORT export_mobile_robot_robot_manager
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install(
  DIRECTORY include/
  DESTINATION include
)

ament_export_include_directories(include)

ament_export_libraries(base_robot_manager_node)

ament_export_targets(export_base_robot_manager HAS_LIBRARY_TARGET)

ament_export_libraries(mobile_robot_robot_manager_node)

ament_export_targets(export_mobile_robot_robot_manager HAS_LIBRARY_TARGET)


if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
