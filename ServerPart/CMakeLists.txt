cmake_minimum_required(VERSION 2.8)

project(ServerPart)

add_library(server STATIC "server.cpp")

add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries(ServerPart server)
