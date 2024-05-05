project(client)
cmake_minimum_required(VERSION 3.22)

find_package(Boost REQUIRED)
find_package(Qt6 REQUIRED)

add_executable(
    ${PROJECT_NAME} 
    src/data_table.cpp
    src/data_table.hpp
    src/main.cpp
    src/models.cpp
    src/models.hpp
    src/requests.cpp
    src/requests.hpp
    src/results_table.cpp
    src/results_table.hpp
)
target_link_libraries(${PROJECT_NAME} boost::boost Qt6::Core Qt6::Widgets)

