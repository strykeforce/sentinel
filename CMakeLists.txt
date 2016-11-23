cmake_minimum_required(VERSION 3.2)
project(Sentinel)

set(WPI_LIBS /opt/wpilib CACHE PATH "WPILib directory.")

set(VERSION_MAJOR 16)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)
set(VERSION_META "-dev")

set(CMAKE_CXX_COMPILER /usr/bin/arm-frc-linux-gnueabi-g++)
set(CMAKE_LD           /usr/bin/arm-frc-linux-gnueabi-ld)
set(CMAKE_AR           /usr/bin/arm-frc-linux-gnueabi-ar)
set(CMAKE_RANLIB       /usr/bin/arm-frc-linux-gnueabi-ranlib)
set(CMAKE_OBJCOPY      /usr/bin/arm-frc-linux-gnueabi-objcopy)
set(CMAKE_OBJCOPY      /usr/bin/arm-frc-linux-gnueabi-objdump)
set(CMAKE_NM           /usr/bin/arm-frc-linux-gnueabi-nm)
set(CMAKE_STRIP        /usr/bin/arm-frc-linux-gnueabi-strip)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -fstrict-aliasing -std=c++1y")

set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

include_directories(
  SYSTEM
  "${WPI_LIBS}/include"
  "${CMAKE_SOURCE_DIR}/vendor/include"
)

link_directories(
  "${WPI_LIBS}/lib"
)

add_subdirectory(src)
