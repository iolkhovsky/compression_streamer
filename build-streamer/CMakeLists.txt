cmake_minimum_required(VERSION 3.10)

project(UdpStreamer)
find_package(OpenCV REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(VARIABLE APP_SOURCES)
set(APP_SOURCES 
${CMAKE_CURRENT_SOURCE_DIR}/../compression_streamer/arg_parser.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/../compression_streamer/client.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../compression_streamer/configurator.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../compression_streamer/cv_video_source.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../compression_streamer/ipc_manager.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../compression_streamer/ipc_video_source.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../compression_streamer/posix_mqueue.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../compression_streamer/posix_semaphore.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../compression_streamer/posix_shmem.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../compression_streamer/server.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../compression_streamer/stream_timing_stat.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../compression_streamer/stream_traffic_stat.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../compression_streamer/transport_protocol.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../compression_streamer/video_codec.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../compression_streamer/video_receiver.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../compression_streamer/video_source.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../compression_streamer/video_streamer.cpp )

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../compression_streamer" ${OpenCV_INCLUDE_DIRS})
add_executable(udp_compression_streamer ${CMAKE_CURRENT_SOURCE_DIR}/../compression_streamer/main.cpp ${APP_SOURCES})
target_link_libraries(udp_compression_streamer ${OpenCV_LIBS})
target_link_libraries(udp_compression_streamer rt)
target_link_libraries(udp_compression_streamer pthread)
