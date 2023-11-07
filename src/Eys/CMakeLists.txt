cmake_minimum_required(VERSION 3.10)
project(Eys)

option(CROSS_COMPILE_ARM "Cross compile for ARM" OFF)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_SYSTEM_NAME Linux)

set(OpenCV_STATIC ON)
add_compile_options(-Wall -Wextra -Wpedantic)

if(CROSS_COMPILE_ARM)
    set(ARM TRUE)
else()
    set(X86_64 TRUE)
endif()

if(ARM)
    set(CMAKE_SYSTEM_PROCESSOR arm)
    set(CMAKE_BUILD_TYPE Release)
    set(tools /usr)
    set(CMAKE_C_COMPILER ${tools}/bin/arm-linux-gnueabihf-gcc)
    set(CMAKE_CXX_COMPILER ${tools}/bin/arm-linux-gnueabihf-g++)
    add_compile_options(-march=armv7-a -mfpu=neon-vfpv4 -mfloat-abi=hard -mtune=cortex-a72)

    set(OpenCV_DIR /home/fpauli/utils/opencv/build_arm)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
    set(EXECUTABLE_NAME Eys_arm)
else()
    find_package(Freetype REQUIRED)
    find_package(Iconv REQUIRED)
    set(OpenCV_DIR /home/fpauli/tools/opencv-4.5.4/build_pc)
    set(EXECUTABLE_NAME Eys_x64)
endif()

enable_testing()

find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

include_directories(/home/fpauli/utils/googletest/googlemock/include)

add_library(gtest IMPORTED STATIC GLOBAL)
add_library(gtest_main IMPORTED STATIC GLOBAL)

set_target_properties(gtest PROPERTIES
  IMPORTED_LOCATION /home/fpauli/utils/googletest/googlemock/libgmock.a)
set_target_properties(gtest_main PROPERTIES
  IMPORTED_LOCATION /home/fpauli/utils/googletest/googlemock/libgmock_main.a)

set(SOURCE
    main.cpp
    Eys.cpp
    SightGuard.cpp
    WakeUP.cpp
    Sessions.cpp
    Session.cpp
    ThreadPool.cpp
)

add_executable(${EXECUTABLE_NAME} ${SOURCE})

if(ARM)
    target_link_libraries(${EXECUTABLE_NAME} ${OpenCV_LIBS} ${Boost_LIBRARIES} Threads::Threads gtest gtest_main)
else()
    target_link_libraries(${EXECUTABLE_NAME} ${FREETYPE_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES} Threads::Threads)
endif()
