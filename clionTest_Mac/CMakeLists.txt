cmake_minimum_required(VERSION 3.6)
project(clionTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(clionTest ${SOURCE_FILES})

set(CMAKE_PREFIX_PATH "/Users/jessicabaron/Qt/5.7/clang_64")
find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)
find_package(Qt5Sql)
qt5_use_modules(clionTest Core Gui Widgets Sql)