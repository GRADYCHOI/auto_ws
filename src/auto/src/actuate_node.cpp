#include <iostream>
#include <stddef.h>
#include <stdio.h>
extern "C" {
#include "Actuate.h" 
}

#include "rtwtypes.h"
#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float64.hpp>
using namespace std::chrono_literals;
using std::placeholders::_1;

void Actuate_step(void);
void Actuate_initialize(void);
void Actuate_terminate(void);

class actuate : public rclcpp::Node
{
  public:
    actuate()
    : Node("actuate")
    {
      subscription_ = this->create_subscription<std_msgs::msg::Float64>(
      "compute/out", 1, std::bind(&actuate::topic_callback, this, _1));


      publisher_ = this->create_publisher<std_msgs::msg::Float64>("actuate/out", 1);
      timer_ = this->create_wall_timer(
      4000ms, std::bind(&actuate::timer_callback, this));
    }

  private:
    void topic_callback(const std_msgs::msg::Float64::SharedPtr msg) const
    {
      Actuate_U.in = msg->data;
      Actuate_step();
      //RCLCPP_INFO(this->get_logger(), "I heard: '%f'", msg->data);

    }
    void timer_callback()
    {
      auto message = std_msgs::msg::Float64();
      message.data = Actuate_Y.out;
      RCLCPP_INFO(this->get_logger(), "Publishing: '%f'", message.data);
      publisher_->publish(message);
    }
    rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr subscription_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;
};

int main(int argc, char * argv[])
{
  Actuate_initialize();

  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<actuate>());
  rclcpp::shutdown();
  
  Actuate_terminate();

  return 0;
}
