#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>

#include <cv_bridge/cv_bridge.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/image_encodings.hpp"
#include "sensor_msgs/msg/image.hpp"
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <sensor_msgs/msg/point_field.hpp>
#include <sensor_msgs/point_cloud2_iterator.hpp>

using namespace std;
using namespace cv;

static const std::string OPENCV_WINDOW = "Image window";

template <typename ROSMessageType> class MagicSubscriber {
public:
  MagicSubscriber(const rclcpp::Node::SharedPtr &node,
                  const std::string &subscriber_topic);

  ~MagicSubscriber();

  void CallbackToTopic(const typename ROSMessageType::SharedPtr msg);

private:
  rclcpp::Node::SharedPtr node_;
  std::string subscriber_topic_;

  typename rclcpp::Subscription<ROSMessageType>::SharedPtr subscriber_;
};

// Template Definitions

template <typename ROSMessageType>
MagicSubscriber<ROSMessageType>::MagicSubscriber(
    const rclcpp::Node::SharedPtr &node, const std::string &subscriber_topic)
    : node_(node), subscriber_topic_(subscriber_topic) {

  subscriber_ = node_->create_subscription<ROSMessageType>(
      subscriber_topic_, rclcpp::SensorDataQoS(),
      std::bind(&MagicSubscriber<ROSMessageType>::CallbackToTopic, this,
                std::placeholders::_1));
}

template <typename ROSMessageType>
MagicSubscriber<ROSMessageType>::~MagicSubscriber() {
  std::cout << "MagicSubscriber Destructor is called" << std::endl;
}

template <typename ROSMessageType>
void MagicSubscriber<ROSMessageType>::CallbackToTopic(
    const typename ROSMessageType::SharedPtr msg) {

  RCLCPP_INFO_STREAM(node_->get_logger(),
                     "GENERIC Template Callback message = " << msg.get());
}

// Image specialization

template <> class MagicSubscriber<sensor_msgs::msg::Image> {
public:
  MagicSubscriber(const rclcpp::Node::SharedPtr &node,
                  const std::string &subscriber_topic,
                  const std::string &publisher_topic)
      : node_(node), subscriber_topic_(subscriber_topic),
        publisher_topic_(publisher_topic) {

    subscriber_ = node_->create_subscription<sensor_msgs::msg::Image>(
        subscriber_topic_, rclcpp::SensorDataQoS(),
        std::bind(&MagicSubscriber<sensor_msgs::msg::Image>::CallbackToTopic,
                  this, std::placeholders::_1));

    publisher_ = node_->create_publisher<sensor_msgs::msg::Image>(
        publisher_topic_, rclcpp::SensorDataQoS());

    cv::namedWindow(OPENCV_WINDOW);

    RCLCPP_INFO(node_->get_logger(), "Image subscriber and publisher created.");
  }

  ~MagicSubscriber() {
    destroyWindow(OPENCV_WINDOW);
    cout << "MagicSubscriber Destructor is called" << endl;
  }

  void CallbackToTopic(const sensor_msgs::msg::Image::SharedPtr msg) {
    RCLCPP_INFO_STREAM(
        node_->get_logger(),
        "Call Back Topic Image Data[0]=" << static_cast<int>(msg->data[0]));

    cv_bridge::CvImagePtr cv_ptr;

    try {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    } catch (const cv_bridge::Exception &e) {
      RCLCPP_ERROR(node_->get_logger(), "cv_bridge exception: %s", e.what());
      return;
    }

    // Convert to grayscale
    cv::cvtColor(cv_ptr->image, cv_ptr->image, cv::COLOR_BGR2GRAY);

    // Blur image
    cv::GaussianBlur(cv_ptr->image, cv_ptr->image, cv::Size(3, 3), 0);

    // Canny edge detector
    cv::Canny(cv_ptr->image, cv_ptr->image, 100, 200, 3, false);

    cv::putText(cv_ptr->image, "EDGE DETECTION CAM", cv::Point(30, 30),
                cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(200, 200, 250),
                1, cv::LINE_AA);

    cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    cv::waitKey(3);

    // Convert back to BGR before publishing
    cv::cvtColor(cv_ptr->image, cv_ptr->image, cv::COLOR_GRAY2BGR);

    publisher_->publish(*cv_ptr->toImageMsg());
  }

private:
  rclcpp::Node::SharedPtr node_;

  std::string subscriber_topic_;
  std::string publisher_topic_;

  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscriber_;

  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_;
};

// PointCloud2 specialization

template <> class MagicSubscriber<sensor_msgs::msg::PointCloud2> {
public:
  MagicSubscriber(const rclcpp::Node::SharedPtr &node,
                  const std::string &subscriber_topic,
                  const std::string &publisher_topic)
      : node_(node), subscriber_topic_(subscriber_topic),
        publisher_topic_(publisher_topic) {

    node_->declare_parameter<int>("image_width", 640);
    node_->declare_parameter<int>("image_height", 480);
    node_->declare_parameter<int>("point_radius", 4);

    /*
     * Camera-like projection looking along +X:
     *
     * image_u = fx * (Y / X) + cx
     * image_v = fy * (-Z / X) + cy
     *
     * X is also used as the depth value.
     */
    node_->declare_parameter<double>("focal_yz_x", 270.0);
    node_->declare_parameter<double>("focal_yz_y", 270.0);
    node_->declare_parameter<double>("centre_u", 319.5);
    node_->declare_parameter<double>("centre_v", 239.5);

    node_->declare_parameter<double>("minimum_x_depth", 0.05);
    node_->declare_parameter<double>("maximum_x_depth", 20.0);

    node_->declare_parameter<bool>("auto_depth_colour_range", true);
    node_->declare_parameter<bool>("invert_horizontal", false);
    node_->declare_parameter<bool>("invert_vertical", false);

    image_width_ = std::max(
        1, static_cast<int>(node_->get_parameter("image_width").as_int()));

    image_height_ = std::max(
        1, static_cast<int>(node_->get_parameter("image_height").as_int()));

    point_radius_ = std::max(
        0, static_cast<int>(node_->get_parameter("point_radius").as_int()));

    focal_u_ =
        static_cast<float>(node_->get_parameter("focal_yz_x").as_double());

    focal_v_ =
        static_cast<float>(node_->get_parameter("focal_yz_y").as_double());

    centre_u_ =
        static_cast<float>(node_->get_parameter("centre_u").as_double());

    centre_v_ =
        static_cast<float>(node_->get_parameter("centre_v").as_double());

    minimum_x_depth_ =
        static_cast<float>(node_->get_parameter("minimum_x_depth").as_double());

    maximum_x_depth_ =
        static_cast<float>(node_->get_parameter("maximum_x_depth").as_double());

    auto_depth_colour_range_ =
        node_->get_parameter("auto_depth_colour_range").as_bool();

    invert_horizontal_ = node_->get_parameter("invert_horizontal").as_bool();

    invert_vertical_ = node_->get_parameter("invert_vertical").as_bool();

    RCLCPP_INFO(node_->get_logger(),
                "Projection along +X: U=fx*(Y/X)+cx, V=fy*(-Z/X)+cy");

    RCLCPP_INFO(node_->get_logger(),
                "Image=%dx%d, focal=(%.1f, %.1f), centre=(%.1f, %.1f)",
                image_width_, image_height_, focal_u_, focal_v_, centre_u_,
                centre_v_);

    subscriber_ = node_->create_subscription<sensor_msgs::msg::PointCloud2>(
        subscriber_topic_, rclcpp::SensorDataQoS(),
        std::bind(
            &MagicSubscriber<sensor_msgs::msg::PointCloud2>::CallbackToTopic,
            this, std::placeholders::_1));

    publisher_ = node_->create_publisher<sensor_msgs::msg::Image>(
        publisher_topic_, rclcpp::SensorDataQoS());
  }

  void CallbackToTopic(const sensor_msgs::msg::PointCloud2::SharedPtr msg) {
    if (!msg || msg->data.empty()) {
      RCLCPP_WARN_THROTTLE(node_->get_logger(), *node_->get_clock(), 5000,
                           "Received an empty PointCloud2 message");
      return;
    }

    if (!hasFloat32XYZFields(*msg)) {
      RCLCPP_ERROR_THROTTLE(
          node_->get_logger(), *node_->get_clock(), 5000,
          "PointCloud2 must contain FLOAT32 x, y and z fields");
      return;
    }

    /*
     * First pass: find the valid X depth range for the colour scale.
     */
    float observed_x_min = std::numeric_limits<float>::max();
    float observed_x_max = std::numeric_limits<float>::lowest();
    std::size_t finite_points = 0;

    try {
      sensor_msgs::PointCloud2ConstIterator<float> iter_x(*msg, "x");
      sensor_msgs::PointCloud2ConstIterator<float> iter_y(*msg, "y");
      sensor_msgs::PointCloud2ConstIterator<float> iter_z(*msg, "z");

      for (; iter_x != iter_x.end(); ++iter_x, ++iter_y, ++iter_z) {
        const float x = *iter_x;
        const float y = *iter_y;
        const float z = *iter_z;

        if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(z)) {
          continue;
        }

        if (x < minimum_x_depth_ || x > maximum_x_depth_) {
          continue;
        }

        observed_x_min = std::min(observed_x_min, x);
        observed_x_max = std::max(observed_x_max, x);
        ++finite_points;
      }
    } catch (const std::runtime_error &exception) {
      RCLCPP_ERROR(node_->get_logger(), "Unable to read PointCloud2 fields: %s",
                   exception.what());
      return;
    }

    if (finite_points == 0) {
      RCLCPP_WARN_THROTTLE(node_->get_logger(), *node_->get_clock(), 3000,
                           "No points have positive X depth in [%.3f, %.3f]. "
                           "The cloud may use a different forward axis.",
                           minimum_x_depth_, maximum_x_depth_);
      return;
    }

    float colour_x_min = minimum_x_depth_;
    float colour_x_max = maximum_x_depth_;

    if (auto_depth_colour_range_) {
      colour_x_min = observed_x_min;
      colour_x_max = observed_x_max;
    }

    if ((colour_x_max - colour_x_min) < 1.0e-5f) {
      colour_x_max = colour_x_min + 1.0f;
    }

    cv::Mat depth_normalized(image_height_, image_width_, CV_8UC1,
                             cv::Scalar(0));

    cv::Mat nearest_x(image_height_, image_width_, CV_32FC1,
                      cv::Scalar(std::numeric_limits<float>::infinity()));

    std::size_t projected_points = 0;
    std::size_t outside_image = 0;

    try {
      sensor_msgs::PointCloud2ConstIterator<float> iter_x(*msg, "x");
      sensor_msgs::PointCloud2ConstIterator<float> iter_y(*msg, "y");
      sensor_msgs::PointCloud2ConstIterator<float> iter_z(*msg, "z");

      for (; iter_x != iter_x.end(); ++iter_x, ++iter_y, ++iter_z) {
        const float x = *iter_x;
        const float y = *iter_y;
        const float z = *iter_z;

        if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(z)) {
          continue;
        }

        /*
         * Looking along +X means only positive X values are in front
         * of the virtual camera.
         */
        if (x < minimum_x_depth_ || x > maximum_x_depth_) {
          continue;
        }

        float projected_u = focal_u_ * (y / x) + centre_u_;

        float projected_v = focal_v_ * (-z / x) + centre_v_;

        if (invert_horizontal_) {
          projected_u = static_cast<float>(image_width_ - 1) - projected_u;
        }

        if (invert_vertical_) {
          projected_v = static_cast<float>(image_height_ - 1) - projected_v;
        }

        const int pixel_u = static_cast<int>(std::lround(projected_u));

        const int pixel_v = static_cast<int>(std::lround(projected_v));

        if (pixel_u < 0 || pixel_u >= image_width_ || pixel_v < 0 ||
            pixel_v >= image_height_) {
          ++outside_image;
          continue;
        }

        const float normalized_depth = std::clamp(
            (x - colour_x_min) / (colour_x_max - colour_x_min), 0.0f, 1.0f);

        /*
         * Reserve value 0 for pixels with no point.
         */
        const std::uint8_t depth_value =
            static_cast<std::uint8_t>(1.0f + normalized_depth * 254.0f);

        drawProjectedPoint(depth_normalized, nearest_x, pixel_u, pixel_v, x,
                           depth_value);

        ++projected_points;
      }
    } catch (const std::runtime_error &exception) {
      RCLCPP_ERROR(node_->get_logger(), "Unable to project PointCloud2: %s",
                   exception.what());
      return;
    }

    cv::Mat colour_image;
    cv::applyColorMap(depth_normalized, colour_image, cv::COLORMAP_TURBO);

    /*
     * applyColorMap colours zero as well, so restore empty pixels to black.
     */
    cv::Mat empty_mask;
    cv::compare(depth_normalized, cv::Scalar(0), empty_mask, cv::CMP_EQ);

    colour_image.setTo(cv::Scalar(0, 0, 0), empty_mask);

    auto output_image =
        cv_bridge::CvImage(msg->header, sensor_msgs::image_encodings::BGR8,
                           colour_image)
            .toImageMsg();

    publisher_->publish(*output_image);

    RCLCPP_INFO_THROTTLE(
        node_->get_logger(), *node_->get_clock(), 2000,
        "Valid X-depth points=%zu, projected=%zu, outside=%zu, "
        "colour X range=[%.3f, %.3f]",
        finite_points, projected_points, outside_image, colour_x_min,
        colour_x_max);
  }

private:
  static bool
  hasFloat32XYZFields(const sensor_msgs::msg::PointCloud2 &message) {
    bool has_x = false;
    bool has_y = false;
    bool has_z = false;

    for (const auto &field : message.fields) {
      if (field.datatype != sensor_msgs::msg::PointField::FLOAT32) {
        continue;
      }

      if (field.name == "x") {
        has_x = true;
      } else if (field.name == "y") {
        has_y = true;
      } else if (field.name == "z") {
        has_z = true;
      }
    }

    return has_x && has_y && has_z;
  }

  void drawProjectedPoint(cv::Mat &depth_image, cv::Mat &nearest_x_image,
                          int centre_u, int centre_v, float x_depth,
                          std::uint8_t depth_value) const {
    for (int offset_v = -point_radius_; offset_v <= point_radius_; ++offset_v) {
      const int pixel_v = centre_v + offset_v;

      if (pixel_v < 0 || pixel_v >= depth_image.rows) {
        continue;
      }

      for (int offset_u = -point_radius_; offset_u <= point_radius_;
           ++offset_u) {
        const int pixel_u = centre_u + offset_u;

        if (pixel_u < 0 || pixel_u >= depth_image.cols) {
          continue;
        }

        if ((offset_u * offset_u + offset_v * offset_v) >
            (point_radius_ * point_radius_)) {
          continue;
        }

        float &stored_x = nearest_x_image.at<float>(pixel_v, pixel_u);

        /*
         * If multiple points project to the same pixel, retain the
         * closest point along X.
         */
        if (x_depth < stored_x) {
          stored_x = x_depth;

          depth_image.at<std::uint8_t>(pixel_v, pixel_u) = depth_value;
        }
      }
    }
  }

  rclcpp::Node::SharedPtr node_;
  std::string subscriber_topic_;
  std::string publisher_topic_;
  rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr subscriber_;

  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_;

  int image_width_ = 640;
  int image_height_ = 480;
  int point_radius_ = 3;

  float focal_u_ = 320.0f;
  float focal_v_ = 320.0f;
  float centre_u_ = 319.5f;
  float centre_v_ = 239.5f;

  float minimum_x_depth_ = 0.05f;
  float maximum_x_depth_ = 20.0f;

  bool auto_depth_colour_range_ = true;
  bool invert_horizontal_ = false;
  bool invert_vertical_ = false;
};