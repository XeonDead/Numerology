cmake_minimum_required(VERSION 3.5)

project(Numerology LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED Charts REQUIRED PATHS E:/Qt/5.12.7/msvc2017_64/lib/cmake)

if (ANDROID)
    add_library(Numerology SHARED
          main.cpp
          AndroidMainWindow.cpp
          AndroidMainWindow.h
          AndroidMainWindow.ui
          Numerology/Numerology.cpp
          Numerology/Numerology.h
          Numerology/CulminationModel.cpp
          Numerology/CulminationModel.h
          Numerology/DayNumberModel.cpp
          Numerology/DayNumberModel.h
    )
    set(HEADERS Numerology)

    link_directories(/opt/android-libs/aarch64)

    find_library(zlib z)
    find_library(log-lib log)

    find_path(GLES2_INCLUDE_DIR GLES2/gl2.h
        HINTS ${ANDROID_NDK})
    find_library(GLES2_LIBRARY libGLESv2.so
        HINTS ${GLES2_INCLUDE_DIR}/../lib)
    target_include_directories(Numerology PUBLIC ${GLES2_INCLUDE_DIR})

    set(ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android)

    target_link_libraries(Numerology PRIVATE Qt5::Widgets Qt5::Charts ${GLES2_LIBRARY} z log)
else()
    add_executable(Numerology
      main.cpp
      MainWindow.cpp
      MainWindow.h
      MainWindow.ui
      Numerology/Numerology.cpp
      Numerology/Numerology.h
      Numerology/CulminationModel.cpp
      Numerology/CulminationModel.h
      Numerology/DayNumberModel.cpp
      Numerology/DayNumberModel.h
    )
    set(HEADERS Numerology)

    target_link_libraries(Numerology PRIVATE Qt5::Widgets Qt5::Charts)
endif()

file(COPY resources/ DESTINATION resources/)

target_compile_definitions(Numerology PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
