#include <cstdint>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <fcntl.h>
#include "robohachi_udp.hpp"

//コンストラクタ
Ros2UDP::Ros2UDP(const std::string& f7_address, int f7_port){
    //ソケットを作成する。引数:(int domain, int type, int protocol)
    //domain: プロトコルファミリ(今回はAF_INET(IPv4)を選択)
    //type: ソケットのタイプ(今回はUDPを使うため、データグラムソケットを作成)
    //protocol: 使用プロトコルを設定。基本ソケットのタイプに従う0を入力。　
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    // ソケットオプションを設定: SO_REUSEADDR
    int yes = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
    }
    
    // ノンブロッキングモードに設定
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);
    
    f7_addr.sin_family = AF_INET;                               //アドレスのプロトコルファミリを選択
    f7_addr.sin_addr.s_addr = inet_addr(f7_address.c_str());    //アドレスを設定
    f7_addr.sin_port = htons(f7_port);
}

//ソケットにアドレスをバインド
void Ros2UDP::udp_bind(){
    if (bind(sock, (const struct sockaddr *)&f7_addr, sizeof(f7_addr)) < 0) {
        perror("Bind failed");
    }
}

//パケットを送信する
void Ros2UDP::send_packet(uint8_t *packet, uint8_t size){
    if (sendto(sock, packet, size, 0, (struct sockaddr *)&f7_addr, sizeof(f7_addr)) < 0) {
        perror("Sendto failed");
    }
}

//パケットを受信する
ssize_t Ros2UDP::udp_recv(uint8_t *buf, uint8_t size){
    memset(buf, 0, size);
    struct sockaddr_in sender_addr;
    socklen_t sender_len = sizeof(sender_addr);
    
    // recvfromを使用して送信元情報も取得
    return recvfrom(sock, buf, size, 0, (struct sockaddr *)&sender_addr, &sender_len);
}