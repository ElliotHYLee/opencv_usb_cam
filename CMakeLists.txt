cmake_minimum_required(VERSION 2.8.3)
project(opencv_usb_cam)

find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  roscpp
  sensor_msgs
  std_msgs
  image_transport
  message_generation
)

find_package(OpenCV REQUIRED)

generate_messages(DEPENDENCIES std_msgs)

catkin_package(
 CATKIN_DEPENDS roscpp std_msgs
)

include_directories(
  ${OpenCV_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}_node src/opencv_usb_cam.cpp  src/detect_line.cpp)

target_link_libraries(${PROJECT_NAME}_node
  ${OpenCV_LIBRARIES}
  ${catkin_LIBRARIES}
)
