#ifndef MAGIC_SUBSCRIBER_IMAGE_EDGEDETECTOR_HPP
#define MAGIC_SUBSCRIBER_IMAGE_EDGEDETECTOR_HPP

#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include <cv_bridge/cv_bridge.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/image_encodings.hpp"
#include "sensor_msgs/msg/image.hpp"

static const std::string OPENCV_WINDOW = "Image window";

using namespace std;
using namespace cv;

class MagicSubscriber {
public:
  MagicSubscriber() { cout << "MagicSubscriber Constructor is called" << endl; }

  ~MagicSubscriber() {
    destroyWindow(OPENCV_WINDOW);
    cout << "MagicSubscriber Destructor is called" << endl;
  }

  template <typename ROSMessageType>
  void init(const rclcpp::Node::SharedPtr &node,
            const std::string &subscriber_topic) {
    m_node_ = node;
    m_subscriber_topic = subscriber_topic;

    image_sub_ = m_node_->create_subscription<ROSMessageType>(
        m_subscriber_topic, rclcpp::SensorDataQoS(),
        std::bind(&MagicSubscriber::CallbackToTopic<ROSMessageType>, this,
                  std::placeholders::_1));

    image_pub_ = m_node_->create_publisher<sensor_msgs::msg::Image>(
        "/image_converter/output_video", 10);

    cv::namedWindow(OPENCV_WINDOW);

    RCLCPP_INFO(m_node_->get_logger(),
                "Image subscriber and publisher created.");
  }

  template <typename CallBackROSMessageType>
  void CallbackToTopic(const typename CallBackROSMessageType::SharedPtr msg) {
    RCLCPP_INFO_STREAM(
        m_node_->get_logger(),
        "Call Back Topic Image Data[0]=" << static_cast<int>(msg->data[0]));

    cv_bridge::CvImagePtr cv_ptr;

    try {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    } catch (const cv_bridge::Exception &e) {
      RCLCPP_ERROR(m_node_->get_logger(), "cv_bridge exception: %s", e.what());
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

    image_pub_->publish(*cv_ptr->toImageMsg());
  }

private:
  rclcpp::Node::SharedPtr m_node_;

  std::string m_subscriber_topic;

  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr image_sub_;

  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr image_pub_;
};

#endif
