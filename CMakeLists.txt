cmake_minimum_required(VERSION 3.6)
project(flohmarkt)

set(CMAKE_CXX_STANDARD 14)

add_executable(flohmarkt program/main.cpp program/solver.cpp program/util.cpp)
target_link_libraries(flohmarkt -std=c++14)

install(TARGETS flohmarkt DESTINATION bin)

