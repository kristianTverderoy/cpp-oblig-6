cmake_minimum_required(VERSION 3.10)

project(oblig6 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -Wextra")

find_package(boost_asio REQUIRED CONFIG)

add_executable(task1 "${CMAKE_CURRENT_SOURCE_DIR}/task1.cpp")
add_executable(task2 "${CMAKE_CURRENT_SOURCE_DIR}/task2.cpp")
target_link_libraries(task2 PRIVATE Boost::asio)
if(MINGW)
  target_link_options(task1 PRIVATE -static-libgcc -static-libstdc++)
  target_link_options(task2 PRIVATE -static-libgcc -static-libstdc++)
endif()
if(WIN32)
  target_link_libraries(task2 PRIVATE ws2_32 mswsock)
endif()
