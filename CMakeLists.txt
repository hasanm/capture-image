cmake_minimum_required(VERSION 3.16)
project(CaptureImage VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_PREFIX_PATH  "${CMAKE_PREFIX_PATH};c:/Qt/6.5.2/msvc2019_64;c:/opencv")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)
find_package(OpenCV REQUIRED )

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable( ${CMAKE_PROJECT_NAME}
                WIN32
                my.qrc
                IntervalBox.cpp
                main.cpp
                mainwindow.cpp)


target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${OpenCV_INCLUDE_DIRS}")

target_link_libraries(${CMAKE_PROJECT_NAME} Qt6::Core Qt6::Widgets Qt6::Gui user32.lib gdi32.lib comdlg32.lib advapi32.lib "${OpenCV_LIBS}") 



