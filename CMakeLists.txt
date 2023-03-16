cmake_minimum_required( VERSION 3.9 )
project (rasplcd VERSION 0.0.1 DESCRIPTION "Raspberry I2C Lcd Driver")

if (NOT CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    message(STATUS "This project has a top-level one called [${CMAKE_PROJECT_NAME}]")
else()
    message(STATUS "This project is a top-level one")
endif()

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(rasplcd SHARED src/rasplcd.cpp)

install(TARGETS rasplcd DESTINATION /usr/lib)
install(FILES include/rasplcd.h DESTINATION include)