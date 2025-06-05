#include <cstdint>
#include <chrono>
#include <memory>
#include <string>
#include "robohachi_udp.hpp"
#include "rclcpp/rclcpp.hpp"
#include "stdint.h"
#define PACKET_SIZE 16
#define F7_ADDR "192.168.21.111"
#define F7_PORT 4001

using namespace std::chrono;
class SendUDP : public rclcpp::Node
{
  public:
    SendUDP(const std::string& f7_address, int f7_port)
    : Node("send_udp"),f7_udp(f7_address,f7_port)    {
        //送信するデータの初期化(今回は全部１にしてみる)
        for(int i=0;i<PACKET_SIZE;i++){
            packet[i] = 1;
        }

        //タイマーで時間ごとにコールバックされる関数を設定
        timer_ = this->create_wall_timer(100ms, std::bind(&SendUDP::timer_callback, this));
    }

  private:
    rclcpp::TimerBase::SharedPtr timer_;
    Ros2UDP f7_udp;
    uint8_t packet[PACKET_SIZE];

    
    void timer_callback(){
        //送信処理(ros2_udp.cppで定義)
        f7_udp.send_packet(packet, sizeof(uint8_t)*PACKET_SIZE);
        //送信したデータを表示
        RCLCPP_INFO(this->get_logger(),"send: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",packet[0],packet[1],packet[2],packet[3],packet[4],packet[5],packet[6],packet[7],packet[8],packet[9],packet[10],packet[11],packet[12],packet[13],packet[14],packet[15]);
    }
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SendUDP>(F7_ADDR,F7_PORT);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}