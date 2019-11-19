cmake_minimum_required(VERSION 3.5)

project(Numerology LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED Charts REQUIRED)

add_executable(Numerology
  main.cpp
  MainWindow.cpp
  MainWindow.h
  MainWindow.ui
  Numerology/Numerology.cpp
  Numerology/Numerology.h
)

set(HEADERS Numerology)

target_link_libraries(Numerology PRIVATE Qt5::Widgets Qt5::Charts)
