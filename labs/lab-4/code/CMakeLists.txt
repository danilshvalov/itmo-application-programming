project(client)
cmake_minimum_required(VERSION 3.22)

find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED)
find_package(Qt6 REQUIRED)

add_executable(
    ${PROJECT_NAME} 
    src/main.cpp
    src/main_window.cpp
    src/main_window.hpp
    src/server.cpp
    src/server.hpp
    src/certificates.cpp
    src/certificates.hpp
)
target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
    boost::boost 
    Qt6::Core 
    Qt6::Widgets 
    OpenSSL::SSL
)
