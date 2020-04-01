cmake_minimum_required(VERSION 3.10)

project(leviathanK)
set(SRC hello.c)

add_executable(${PROJECT_NAME} ${SRC})
set_property(TARGET leviathanK PROPERTY C_STANDARD 11)
set_property(TARGET leviathanK PROPERTY C_STANDARD_REQUIRED ON)