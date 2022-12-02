cmake_minimum_required(VERSION 3.16)
project(tcp-server-client-ping-pong-timing)

set(CMAKE_CXX_STANDARD 14)

add_executable(server
        server.cpp)

add_executable(client
        client.cpp)
