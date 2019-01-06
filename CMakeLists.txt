cmake_minimum_required(VERSION 3.12)
project(Bezier)

set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DVERBOSE")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(
        src)

# Source files
set(src src/main.cpp

        src/util/code_organizers.h

        src/util/math.h
        src/util/math.cpp

        src/logic.h src/logic.cpp
        src/Bezier.h src/Bezier.cpp

        src/BasicPoint.h src/BasicPoint.cpp
        src/Polygon.cpp src/Polygon.h
        )

# exe
add_executable(${PROJECT_NAME} ${src})
target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})

set(bezier_lib ${PROJECT_NAME}_lib)
add_library(${bezier_lib} ${src})

include(CTest)

# tests
add_subdirectory(tests)