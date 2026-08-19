cmake_minimum_required(VERSION 3.8)
project(exercises_unit_2)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(robot_commander REQUIRED)

# Create executable
add_executable(robot_position_array_executable src/robot_position_as_array.cpp)
add_executable(robot_heading_vector_executable src/robot_heading_as_vector.cpp)
add_executable(robot_heading_deque_executable src/robot_heading_as_deque.cpp)
add_executable(path_coordinates_executable src/path_coordinates.cpp)
add_executable(path_coordinates_map_executable src/path_coordinates_map.cpp)

# Add dependencies
ament_target_dependencies(robot_position_array_executable
  rclcpp
  robot_commander
)

ament_target_dependencies(robot_heading_vector_executable
  rclcpp
  robot_commander
)

ament_target_dependencies(robot_heading_deque_executable
  rclcpp
  robot_commander
)

ament_target_dependencies(path_coordinates_executable
  rclcpp
  robot_commander
)

ament_target_dependencies(path_coordinates_map_executable
  rclcpp
  robot_commander
)

# Install executables
install(TARGETS
  robot_position_array_executable
  robot_heading_vector_executable
  robot_heading_deque_executable
  path_coordinates_executable
  path_coordinates_map_executable
  DESTINATION lib/${PROJECT_NAME})

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
