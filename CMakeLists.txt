cmake_minimum_required(VERSION 3.18)
project(ImageStudio LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(ImageStudio
        main.cpp
        MainWindow.cpp MainWindow.h
        ImageBridge.cpp ImageBridge.h
        filters.h
        Image_Class.h stb_image.h stb_image_write.h
)

target_link_libraries(ImageStudio PRIVATE Qt6::Widgets)