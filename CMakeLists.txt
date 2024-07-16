cmake_minimum_required(VERSION 3.23)
project(Minesweeping)

set(CMAKE_CXX_STANDARD 14)
include_directories(./EasyX/include)
link_directories(./EasyX/lib64)
link_libraries(libeasyx.a)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

add_executable(Minesweeping main.cpp)
