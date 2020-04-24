cmake_minimum_required(VERSION 3.10)

project(leviathanK)
set(CMAKE_BUILD_TYPE Debug)
set(LEVI_DIR src/levi)
set(LIB_DIR src/lib)
set(LEVI_SRC ${LEVI_DIR}/shell.c 
             ${LEVI_DIR}/test.c)
set(LIB_SRC ${LIB_DIR}/memory.c
            ${LIB_DIR}/list.c
            ${LIB_DIR}/liblevi.c
            ${LIB_DIR}/path_tree.c
            ${LIB_DIR}/util.c)

include_directories(${PROJECT_SOURCE_DIR}/include)
add_library(libleviathanK SHARED ${LIB_SRC})
set_target_properties(libleviathanK PROPERTIES PREFIX "")

add_executable(${PROJECT_NAME} ${LEVI_SRC})
target_link_libraries(${PROJECT_NAME} libleviathanK)
set_property(TARGET leviathanK PROPERTY C_STANDARD 11)
set_property(TARGET leviathanK PROPERTY C_STANDARD_REQUIRED ON)