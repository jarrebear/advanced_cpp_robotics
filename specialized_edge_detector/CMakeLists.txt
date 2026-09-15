cmake_minimum_required(VERSION 3.8)
project(specialized_edge_detector)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(pcl_conversions REQUIRED)
find_package(OpenCV REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)

##############################################################################
# Library: magic_subscriber_image_edgedetector
##############################################################################

add_library(magic_subscriber_image_edgedetector SHARED
  src/magic_subscriber_image_edgedetector.cpp
)

target_include_directories(magic_subscriber_image_edgedetector
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${OpenCV_INCLUDE_DIRS}
)

ament_target_dependencies(magic_subscriber_image_edgedetector
  rclcpp
  sensor_msgs
  cv_bridge
)

target_link_libraries(magic_subscriber_image_edgedetector
  ${OpenCV_LIBS}
)

##############################################################################
# Library: magic_subscriber_pcl2image
##############################################################################

add_library(magic_subscriber_pcl2image SHARED
  src/magic_subscriber_pcl2image.cpp
)

target_include_directories(magic_subscriber_pcl2image
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${OpenCV_INCLUDE_DIRS}
)

ament_target_dependencies(magic_subscriber_pcl2image
  rclcpp
  sensor_msgs
  cv_bridge
  pcl_conversions
)

target_link_libraries(magic_subscriber_pcl2image
  ${OpenCV_LIBS}
)


##############################################################################
# Library: specialised_complete
##############################################################################

add_library(specialised_complete_lib SHARED
  src/specialised_complete.cpp
)

target_include_directories(specialised_complete_lib
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${OpenCV_INCLUDE_DIRS}
)

ament_target_dependencies(specialised_complete_lib
  rclcpp
  sensor_msgs
  cv_bridge
)

target_link_libraries(specialised_complete_lib
  ${OpenCV_LIBS}
)


##############################################################################
# Executable: main_edge_detector_node
##############################################################################


add_executable(main_edge_detector_node
  src/main_edge_detector.cpp
)

target_link_libraries(main_edge_detector_node
  magic_subscriber_image_edgedetector
)

ament_target_dependencies(main_edge_detector_node
  rclcpp
  sensor_msgs
  cv_bridge
)

##############################################################################
# Executable: main_pcl2image_node
##############################################################################

add_executable(main_pcl2image_node
  src/main_pcl2image.cpp
)

target_link_libraries(main_pcl2image_node
  magic_subscriber_pcl2image
)

ament_target_dependencies(main_pcl2image_node
  rclcpp
  sensor_msgs
  cv_bridge
  pcl_conversions
)


##############################################################################
# Executable: main_specialised_complete_node
##############################################################################


add_executable(main_specialised_complete_node
  src/main_specialised_complete.cpp
)

target_link_libraries(main_specialised_complete_node
  specialised_complete_lib
)

ament_target_dependencies(main_specialised_complete_node
  rclcpp
  sensor_msgs
  cv_bridge
  pcl_conversions
)

##############################################################################
# Installation
##############################################################################

install(
  TARGETS
    magic_subscriber_image_edgedetector
    main_edge_detector_node
	magic_subscriber_pcl2image
	main_pcl2image_node
	specialised_complete_lib
	main_specialised_complete_node
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install(
  DIRECTORY include/
  DESTINATION include
)

ament_export_include_directories(include)

ament_export_libraries(
  magic_subscriber_image_edgedetector
  magic_subscriber_pcl2image
  specialised_complete_lib
#   specialised_complete_lib
)

ament_export_targets(
  export_${PROJECT_NAME}
  HAS_LIBRARY_TARGET
)

ament_export_dependencies(
  rclcpp
  sensor_msgs
  cv_bridge
  pcl_conversions
)


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
