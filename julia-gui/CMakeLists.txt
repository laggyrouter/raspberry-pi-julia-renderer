cmake_minimum_required(VERSION 3.16)

project(JuliaGUI VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(OpenMP REQUIRED)

qt_standard_project_setup()

qt_add_executable(julia_gui
    src/main.cpp
)

target_link_libraries(julia_gui PRIVATE 
    Qt6::Widgets
    OpenMP::OpenMP_CXX
)
