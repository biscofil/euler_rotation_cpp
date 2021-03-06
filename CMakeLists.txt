cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME quaternions)

project(${TARGET_NAME})

set(CMAKE_CXX_STANDARD 14)

add_executable(${TARGET_NAME}
        src/qvm_utils.h
        src/ui/ui.h
        src/ui/ui.cpp
        src/ui/uiHelper.h
        src/ui/uiHelper.cpp
        src/Simulation.h
        src/Simulation.cpp
        src/main.cpp
        )

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(SDL2 REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})
target_link_libraries(${TARGET_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${SDL2_LIBRARIES})

find_package(Threads REQUIRED)
target_link_libraries(${TARGET_NAME} ${CMAKE_THREAD_LIBS_INIT})