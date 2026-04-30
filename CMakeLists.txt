cmake_minimum_required(VERSION 3.10)

project(StockFlow_Backend)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 1. Find the Crow library installed by vcpkg
find_package(Crow CONFIG REQUIRED)

add_executable(main src/main.cpp)

# 2. Link Crow to our executable
target_link_libraries(main PRIVATE Crow::Crow)