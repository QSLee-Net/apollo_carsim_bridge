cmake_minimum_required(VERSION 3.10.2)
project(apollo_carsim_bridge)
## Compile as C++11
add_compile_options(-std=c++11)

## Find libraries
find_package(Protobuf REQUIRED)

###########
## Build ##
###########
## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/common
    ${Protobuf_INCLUDE_DIRS}
)

# Declare a C++ executable
# With catkin_make all packages are built within a single CMake context
# The recommended prefix ensures that target names across packages don't collide
add_library(${PROJECT_NAME} SHARED
            common/bridge_header.cc
            include/carsim_chassis.pb.cc
            include/control_controlcmd.pb.cc
            include/header.pb.cc
            include/error_code.pb.cc
)
add_executable(${PROJECT_NAME}_send src/udp_bridge_sender_carsim.cc)
add_executable(${PROJECT_NAME}_receive src/udp_bridge_receiver_carsim.cc)
# Specify libraries to link a library or executable target against

target_link_libraries(${PROJECT_NAME}_send
  ${PROJECT_NAME}
  ${PROTOBUF_LIBRARIES}
)
target_link_libraries(${PROJECT_NAME}_receive
  ${PROJECT_NAME}
  ${PROTOBUF_LIBRARIES}
)
