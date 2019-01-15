cmake_minimum_required(VERSION 3.12)
project(Bezier)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DVERBOSE")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(
        src/include)

# Source files
set(src
        src/include/util/code_organizers.h

        src/include/util/math.h src/math.cpp

        src/include/BazicPoint.h
        src/include/Polygon.h src/Polygon.cpp

        src/include/logic.h src/logic.cpp)

# exe
add_executable(${PROJECT_NAME} ${src} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})

set(bezier_lib ${PROJECT_NAME}_lib)
add_library(${bezier_lib} ${src})

include(CTest)

# tests
add_subdirectory(tests)