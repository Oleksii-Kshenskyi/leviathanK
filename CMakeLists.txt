cmake_minimum_required(VERSION 3.10)

project(leviathanK)
set(LEVI_SRC hello.c)
set(LIB_SRC libhello.c)

include_directories(${PROJECT_SOURCE_DIR})
add_library(libleviathanK SHARED ${LIB_SRC})

add_executable(${PROJECT_NAME} ${LEVI_SRC})
target_link_libraries(${PROJECT_NAME} libleviathanK)
set_property(TARGET leviathanK PROPERTY C_STANDARD 11)
set_property(TARGET leviathanK PROPERTY C_STANDARD_REQUIRED ON)