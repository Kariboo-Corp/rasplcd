cmake_minimum_required( VERSION 3.9 )
project(ExampleScreen VERSION 0.0.1 DESCRIPTION "Screen Controller")

add_library(rasplcd SHARED IMPORTED)
set_property(TARGET rasplcd PROPERTY IMPORTED_LOCATION "/usr/lib/librasplcd.so")

# it is an application
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} rasplcd)