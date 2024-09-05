cmake_minimum_required(VERSION 3.16.3)

set(NAME mtg-stock-app)

project(
    mtg-sctock-app
    DESCRIPTION "App to save and update your mtg stock into cardmarket"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add_subdirectory(src)
add_subdirectory(vendor)

add_executable(${NAME} src/main.cpp src/application.cpp src/application.hpp src/window.hpp src/window.cpp)

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(${NAME} PRIVATE cxx_std_20)
target_link_libraries(${NAME}
  PUBLIC SDL2::SDL2 imgui)