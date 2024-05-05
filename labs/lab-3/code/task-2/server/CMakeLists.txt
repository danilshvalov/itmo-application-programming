project(server)
cmake_minimum_required(VERSION 3.22)

find_package(Boost REQUIRED)

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
    src/math.cpp
    src/math.hpp
    src/models.cpp
    src/models.hpp
)
target_link_libraries(${PROJECT_NAME} boost::boost)
