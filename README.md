# udp_package

ロボハチで使用するEthernetでUDP通信を行うパッケージです。
詳しくはこの記事を参照して下さい。[Ethernetを使用したロボット通信構成試作](https://robo8.esa.io/posts/540)


クローンしたら以下の手順でビルドして下さい。
```
cd ~/ros2_ws
colcon build
source install/setup.bash
```

送信側コマンド
```
ros2 run udp_package udp_send
```

受信コマンド

```
ros2 run udp_package udp_receive
```
