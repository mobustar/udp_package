cmake_minimum_required(VERSION 3.8)
project(udp_package)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

# 共通のソースとヘッダー
set(common_sources
  src/robohachi_udp.cpp
)

# 送信ノードの実行ファイル
add_executable(udp_send 
  src/send_udp.cpp 
  ${common_sources}
)
ament_target_dependencies(udp_send rclcpp)
target_include_directories(udp_send PRIVATE ./include)

# 受信ノードの実行ファイル
add_executable(udp_receive 
  src/receive_udp.cpp 
  ${common_sources}
)
ament_target_dependencies(udp_receive rclcpp)
target_include_directories(udp_receive PRIVATE ./include)

# インストール設定
install(TARGETS
  udp_send
  udp_receive
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()