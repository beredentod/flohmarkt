cmake_minimum_required(VERSION 3.6)
project(flohmarkt)

set(CMAKE_CXX_STANDARD 17)

add_executable(flohmarkt program/main.cpp program/solver.cpp program/util.cpp)
target_link_libraries(flohmarkt -std=c++17)

install(TARGETS flohmarkt DESTINATION bin)

