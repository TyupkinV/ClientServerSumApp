cmake_minimum_required(VERSION 2.8)

project(ClientPart_)

add_library(client STATIC "client.cpp")

add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries(ClientPart_ client)
