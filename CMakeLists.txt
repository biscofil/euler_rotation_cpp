cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME quaternions)

project(${TARGET_NAME})

set(CMAKE_CXX_STANDARD 14)

add_executable(${TARGET_NAME} main.cpp
        ui.h
        Simulation.h
        Simulation.cpp
        )

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(SDL2 REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})
target_link_libraries(${TARGET_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${SDL2_LIBRARIES})

find_package(Threads REQUIRED)
target_link_libraries(${TARGET_NAME} ${CMAKE_THREAD_LIBS_INIT})