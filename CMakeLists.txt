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
        src/util/Polynomial.h

        src/logic.h src/logic.cpp
        src/Bezier.h

        src/BasicPoint.h src/Bezier.cpp src/Hull.cpp src/Hull.h src/BasicPoint.cpp src/util/Path.h)

# exe
add_executable(${PROJECT_NAME} ${src})
target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})