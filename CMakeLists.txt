cmake_minimum_required(VERSION 3.5)

project(Numerology LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED Charts REQUIRED PATHS E:/Qt/5.12.6/msvc2017_64/lib/cmake)

add_executable(Numerology
  main.cpp
  MainWindow.cpp
  MainWindow.h
  MainWindow.ui
  Numerology/Numerology.cpp
  Numerology/Numerology.h
)

set(HEADERS Numerology)

file(COPY resources/ DESTINATION resources/)

target_link_libraries(Numerology PRIVATE Qt5::Widgets Qt5::Charts)
