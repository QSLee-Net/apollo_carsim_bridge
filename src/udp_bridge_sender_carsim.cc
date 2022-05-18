/******************************************************************************
 * Copyright 2019 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdlib>
#include <thread>
#include <bits/c++io.h>

#include "carsim_chassis.pb.h"
#include "bridge_proto_serialized_buf.h"


bool send(const std::string &remote_ip, uint16_t remote_port, uint32_t count) {
  if (count == 0) {
    count = 10000;
  }
  float total = static_cast<float>(count);
  float hundred = 100.00;
  for (uint32_t i = 0; i < count; i++) {
    double timestamp_ = clock() + 2.0;
    float coefficient = static_cast<float>(i);
    auto pb_msg = std::make_shared<apollo::bridge::Carsim>();
    // carsim output
    pb_msg->set_acc_x_egovehicle(10);
    pb_msg->set_acc_y_egovehicle(2);
    pb_msg->set_angle_pitch_egovehicle(0);
    pb_msg->set_angle_roll_egovehicle(0);
    pb_msg->set_angle_yaw_egovehicle(5);
    pb_msg->set_angle_steering(5);
    pb_msg->set_angle_rate_steering(1);
    pb_msg->set_gear_localtion(apollo::bridge::Carsim_GearPosition::Carsim_GearPosition_GEAR_DRIVE);
    pb_msg->set_pos_x_egovehicle(100);
    pb_msg->set_pos_y_egovehicle(10);
    pb_msg->set_pos_z_egovehicle(0);
    pb_msg->set_speed_mps_vx(100);
    pb_msg->set_speed_mps_vy(5);
    pb_msg->set_yaw_rate(1);

    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = inet_addr(remote_ip.c_str());
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(remote_port);

    std::cout << "connecting to server... " << std::endl;

    int sock_fd = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0);

    int res =
        connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (res < 0) {
      std::cerr << "connected server failed ";
      continue;
    }

    std::cerr << "connected to server success. port [" << remote_port << "]";

    apollo::bridge::BridgeProtoSerializedBuf<apollo::bridge::Carsim> proto_buf;
    proto_buf.Serialize(pb_msg, "carsim");
    for (size_t j = 0; j < proto_buf.GetSerializedBufCount(); j++) {
      ssize_t nbytes = send(sock_fd, proto_buf.GetSerializedBuf(j),
                            proto_buf.GetSerializedBufSize(j), 0);
      if (nbytes != static_cast<ssize_t>(proto_buf.GetSerializedBufSize(j))) {
        std::cerr << "sent msg failed ";
        break;
      }
      std::cerr << "sent " << nbytes << " bytes to server with sequence num " << i;
    }
    close(sock_fd);

    // 1000Hz
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  return true;
}

int main(int argc, char *argv[]) {
  uint32_t count = 0;
  if (argc < 2) {
    count = 10000;
  } else {
    count = atoi(argv[1]);
  }
  send("127.0.0.1", 8900, count);
  return 0;
}
