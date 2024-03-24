cmake_minimum_required(VERSION 3.27)
project(browser-window-popup-application)

set(CMAKE_CXX_STANDARD 23)

set(target_name browser-window-popup)

add_executable(${target_name} main.cpp
    clipboard.hpp
    logger.hpp
    now.hpp
)

set(CMAKE_EXE_LINKER_FLAGS -static)
target_link_options(${target_name} PRIVATE -static-libgcc -static-libstdc++)
# https://stackoverflow.com/questions/19450722
# https://stackoverflow.com/questions/4702732
# https://stackoverflow.com/questions/38694058

configure_file(manifest-config.json manifest.json)
