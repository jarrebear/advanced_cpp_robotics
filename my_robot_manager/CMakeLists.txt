cmake_minimum_required(VERSION 3.8)
project(my_robot_manager)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(nav_msgs REQUIRED)

# Include directories
include_directories(include)

# Create library
add_library(${PROJECT_NAME}
  src/robot_manager.cpp
)

# Create executable
add_executable(robot_manager_node src/robot_manager_node.cpp)

# Link robot_manager_node to my_robot_manager library
target_link_libraries(robot_manager_node
  ${PROJECT_NAME}
)

# Add dependencies
ament_target_dependencies(robot_manager_node
  rclcpp
  nav_msgs
  )

# Link dependencies
ament_target_dependencies(${PROJECT_NAME}
  rclcpp
  nav_msgs
)

# Export include directories
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Install library
install(
  TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

# Install executables
install(TARGETS
  robot_manager_node
  DESTINATION lib/${PROJECT_NAME})

# Install headers
install(
  DIRECTORY include/
  DESTINATION include
)

# Export package dependencies
ament_export_targets(export_my_robot_manager HAS_LIBRARY_TARGET)
ament_export_include_directories(include)
ament_export_dependencies(rclcpp nav_msgs)

ament_package()