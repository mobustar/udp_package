#include "robohachi_udp.hpp"
#include "rclcpp/rclcpp.hpp"
#include <cstdint>
#include <sys/socket.h>
#include <string>
#include <cstring>
#include <stdio.h>
#define BUFFER_SIZE 16
#define F7_PORT 4001

// 送信側と同じポートでリッスンするが、アドレスは任意のインターフェースでリッスン
#define LISTEN_ADDR "0.0.0.0"  

class ReceiveUDP : public rclcpp::Node{
    public:
        ReceiveUDP(const std::string& listen_address, int listen_port)
            :Node("receive_udp"), f7_udp(listen_address, listen_port){
                f7_udp.udp_bind();
                RCLCPP_INFO(this->get_logger(), "UDP Receiver started, listening on %s:%d", 
                           listen_address.c_str(), listen_port);
            }
        void receive_data(){
            uint8_t buf[BUFFER_SIZE] = {0};
            ssize_t received_length = f7_udp.udp_recv(buf, sizeof(buf));

            if(received_length > 0){
                RCLCPP_INFO(this->get_logger(), "Received Data = %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                           buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],buf[8],buf[9],
                           buf[10],buf[11],buf[12],buf[13],buf[14],buf[15],buf[16],buf[17],buf[18],buf[19]);
            } else if (received_length < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
                // ノンブロッキングモードでデータがない場合はEAGAINまたはEWOULDBLOCKが返る
                RCLCPP_ERROR(this->get_logger(), "Failed to receive UDP data: %s", strerror(errno));
            }
            // データがない場合は何もしない（ノンブロッキングモード）
        }
    private:
        Ros2UDP f7_udp;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ReceiveUDP>(LISTEN_ADDR, F7_PORT);
    rclcpp::Rate rate(100); // 100Hzに増やしてポーリング頻度を上げる
    
    while (rclcpp::ok()) {
        node->receive_data();
        rclcpp::spin_some(node); 
        rate.sleep();
    }
    rclcpp::shutdown();
    return 0;
}