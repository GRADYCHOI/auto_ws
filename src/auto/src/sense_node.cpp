#include <iostream>
#include <stddef.h>
#include <stdio.h>
#include "Sense.h"             
#include "rtwtypes.h"
#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float64.hpp>
using namespace std::chrono_literals;

//extern "C" void Sense_step(void);
//extern "C" void Sense_initialize(void);
//extern "C" void Sense_terminate(void);
void Sense_step(void);
void Sense_initialize(void);
void Sense_terminate(void);

class sense : public rclcpp::Node
{
  public:
    sense()
    : Node("sense"), count_(0)
    {
      publisher_ = this->create_publisher<std_msgs::msg::Float64>("sense/out", 1);
      timer_ = this->create_wall_timer(
      1000ms, std::bind(&sense::timer_callback, this));
    }

  private:
    void timer_callback()
    {
      Sense_step();
      auto message = std_msgs::msg::Float64();
      message.data = Sense_Y.out;
      RCLCPP_INFO(this->get_logger(), "Publishing: '%f'", message.data);
      publisher_->publish(message);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;
    size_t count_;
};

int main(int argc, char * argv[])
{
  Sense_initialize();

  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<sense>());
  rclcpp::shutdown();
  
  Sense_terminate();
  
  return 0;
}
