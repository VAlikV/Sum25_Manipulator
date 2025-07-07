cmake_minimum_required(VERSION 3.5.0)

set(PROJECT PointControl)

project(${PROJECT} VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(GCC_COVERAGE_COMPILE_FLAGS "-W -fexceptions -O2 -DNDEBUG -Dlinux")

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(drake CONFIG REQUIRED)

add_executable(${PROJECT}
    main.cpp
    point_control/point_control.cpp     point_control/point_control.hpp
    ik/drake_kinematic.cpp              ik/drake_kinematic.hpp
    # ik/cost.hpp
    udp/udp_server.cpp                  udp/udp_server.hpp
    udp/json.hpp
    lockfree/lockfree.hpp
)

target_link_libraries(${PROJECT}
    # nlohmann_json::nlohmann_json
    Eigen3::Eigen
    drake::drake
)