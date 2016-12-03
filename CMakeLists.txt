cmake_minimum_required(VERSION 3.2)
project(Sentinel)

set(WPI_LIBS /opt/wpilib CACHE PATH "WPILib directory.")
set(SIDEWINDER_LIBS /opt/sidewinder CACHE PATH "Sidewinder directory.")

set(CMAKE_CXX_COMPILER /usr/bin/arm-frc-linux-gnueabi-g++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -fstrict-aliasing -std=c++1y")

set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

include_directories(
  SYSTEM
  "${SIDEWINDER_LIBS}/include"
  "${WPI_LIBS}/include"
  "${CMAKE_SOURCE_DIR}/vendor/include"
)

link_directories(
  "${SIDEWINDER_LIBS}/lib"
  "${WPI_LIBS}/lib"
)

add_subdirectory(src)
