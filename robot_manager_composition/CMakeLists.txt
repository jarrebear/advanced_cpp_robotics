cmake_minimum_required(VERSION 3.8)
project(robot_manager_composition)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_srvs REQUIRED)

# Include header files
include_directories(include)

##################################################
# Library
##################################################

add_library(system_information SHARED
  src/system_information.cpp
)

target_include_directories(system_information
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(system_information
  rclcpp
)

add_library(robot_manager_composition SHARED
  src/robot_manager_composition.cpp
)

target_include_directories(robot_manager_composition
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(robot_manager_composition
  rclcpp
  std_srvs
)

##################################################
# Executable
##################################################

add_executable(system_information_test_executable
  src/system_information_test_executable.cpp
)

target_link_libraries(system_information_test_executable
  system_information
)

ament_target_dependencies(system_information_test_executable
  rclcpp
)

add_executable(robot_manager_composition_node
  src/robot_manager_composition_node.cpp
)

target_link_libraries(robot_manager_composition_node
  robot_manager_composition
)

ament_target_dependencies(robot_manager_composition_node
  rclcpp
  std_srvs
)


##################################################
# Export library
##################################################

install(
  TARGETS
    system_information
    system_information_test_executable
  EXPORT export_system_information
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install(
  TARGETS
	robot_manager_composition
	robot_manager_composition_node
  EXPORT export_robot_manager_composition
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install(
  DIRECTORY include/
  DESTINATION include
)

ament_export_include_directories(include)

ament_export_libraries(system_information)

ament_export_targets(export_system_information HAS_LIBRARY_TARGET)

ament_export_libraries(robot_manager_composition)

ament_export_targets(export_robot_manager_composition HAS_LIBRARY_TARGET)

##################################################
# Package
##################################################

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