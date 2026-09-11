cmake_minimum_required(VERSION 3.8)
project(unit6_exercises)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

##############################################################################
# Dependencies
##############################################################################

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)

##############################################################################
# Library: magic_printer
##############################################################################

add_library(magic_printer SHARED
  src/magic_printer.cpp
)

target_include_directories(magic_printer
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(magic_printer
  rclcpp
)

##############################################################################
# Executable: main_magic_printer_node
##############################################################################

add_executable(main_magic_printer_node
  src/main_magic_printer.cpp
)

target_link_libraries(main_magic_printer_node
  magic_printer
)

ament_target_dependencies(main_magic_printer_node
  rclcpp
)

add_executable(main_topic_subscriber_function
  src/main_topic_subscriber_function.cpp
)

ament_target_dependencies(main_topic_subscriber_function
  rclcpp
  sensor_msgs
)

add_executable(main_topic_subscriber_class
  src/main_topic_subscriber_class.cpp
)

ament_target_dependencies(main_topic_subscriber_class
  rclcpp
  sensor_msgs
)

add_executable(main_pcl_topic_subscriber
  src/main_pcl_topic_subscriber.cpp
)

ament_target_dependencies(main_pcl_topic_subscriber
  rclcpp
  sensor_msgs
)

##############################################################################
# Install Header Files
##############################################################################

install(
  DIRECTORY include/
  DESTINATION include
)

##############################################################################
# Install Libraries
##############################################################################

install(
  TARGETS
    magic_printer
    EXPORT export_${PROJECT_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

##############################################################################
# Install Executables
##############################################################################

install(
  TARGETS
    main_magic_printer_node
    DESTINATION lib/${PROJECT_NAME}
)

install(TARGETS
  main_topic_subscriber_function
  DESTINATION lib/${PROJECT_NAME}
)

install(TARGETS
  main_topic_subscriber_class
  DESTINATION lib/${PROJECT_NAME}
)


install(TARGETS
  main_pcl_topic_subscriber
  DESTINATION lib/${PROJECT_NAME}
)


##############################################################################
# Export
##############################################################################

ament_export_include_directories(include)

ament_export_libraries(
  magic_printer
)

ament_export_targets(
  export_${PROJECT_NAME}
  HAS_LIBRARY_TARGET
)

ament_export_dependencies(
  rclcpp
)

##############################################################################
# Testing
##############################################################################

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

##############################################################################
# Package
##############################################################################

ament_package()
