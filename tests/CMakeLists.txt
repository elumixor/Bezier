project(bezier_tests)


add_subdirectory(google_test)
include_directories(google_test/googletest/include)
#include_directories(google_test/googlemock/include)
include_directories(../../src)


set(src
        main.cpp src/util/math.cpp src/Polygon.cpp
        src/Bezier.cpp src/intersections.cpp src/poly_helper.cpp src/BazicPoint.cpp src/logic.cpp)

add_executable(${PROJECT_NAME} ${src})
target_link_libraries(${PROJECT_NAME} ${polygons_lib} gtest gtest_main)