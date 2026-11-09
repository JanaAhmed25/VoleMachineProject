cmake_minimum_required(VERSION 3.16)
project(VoleMachineGUI VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_executable(VoleMachineGUI
    main.cpp
    MainWindow.cpp
    MainWindow.h
    ../CPU.cpp
    ../Memory.cpp
    ../Screen.cpp
    ../Instruction.cpp
    ../ProgramLoader.cpp
    ../Simulator.cpp
)

target_include_directories(VoleMachineGUI PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(VoleMachineGUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)