#include <iostream>
#include <string>

#include "rclcpp/rclcpp.hpp"

using std::string;

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);

  auto node = std::make_shared<rclcpp::Node>("lambda_basics_node");

  // Prevent unused variable warning
  (void)node;

  // Define the lambda function merge_words
  auto merge_words = [](const std::string &a,
                        const std::string &b) -> std::string {
    return a + "-" + b;
  };

  auto result = merge_words("Adventure", "Time");

  std::cout << "Result Merge: " << result << std::endl;

  auto dummy_positive = []() -> bool { return false; };

  bool dummy_x = dummy_positive();
  printf(dummy_x ? "true" : "false");
  printf("\n");

  // Capture list example
  std::string w1 = "Finn";
  std::string w2 = "Jacke";

  auto best_pals = [w1, &w2] {
    w2 = "IceKing";
    return w1 + " friend of " + w2;
  };
  auto result2 = best_pals();
  std::cout << "Result best_pals: " << result2 << std::endl;
  std::cout << "final value of w1: " << w1 << std::endl;
  std::cout << "final value of w2: " << w2 << std::endl;

  // Capture list example Complex
  std::string w3 = "BMO";
  std::string w4 = "Princess_Bubblegum";
  std::string w5 = "Earl_of_Lemongrab";

  auto party_list_val = [=, &w3] {
    w3 = "IceKing";
    return w3 + ", " + w4 + ", " + w5;
  };
  auto result3 = party_list_val();
  std::cout << "Result party_list Value: " << result3 << std::endl;
  std::cout << "final value of w3: " << w3 << std::endl;
  std::cout << "final value of w4: " << w4 << std::endl;
  std::cout << "final value of w5: " << w5 << std::endl;

  auto party_list_ref = [&, w3] {
    w4 = "Peppermint-Butler";
    return w3 + ", " + w4 + ", " + w5;
  };
  auto result4 = party_list_ref();
  std::cout << "Result party_list Reference: " << result4 << std::endl;
  std::cout << "final value of w3: " << w3 << std::endl;
  std::cout << "final value of w4: " << w4 << std::endl;
  std::cout << "final value of w5: " << w5 << std::endl;

  // Mutable example
  std::string MutableName = "Prismo";
  int MutableId = 1;

  auto mutable_lambda = [=]() mutable {
    MutableId += 1;
    MutableName = "Prismo-" + std::to_string(MutableId);
    std::cout << "MutableName Inside Lambda =" << MutableName << std::endl;
    std::cout << "MutableId Inside Lambda =" << MutableId << std::endl;
    return;
  };
  mutable_lambda();
  std::cout << "MutableName Outside Lambda =" << MutableName << std::endl;
  std::cout << "MutableId Outside Lambda =" << MutableId << std::endl;

  mutable_lambda();
  std::cout << "MutableName Outside Lambda =" << MutableName << std::endl;
  std::cout << "MutableId Outside Lambda =" << MutableId << std::endl;

  rclcpp::shutdown();
  return 0;
}
