cmake_minimum_required(VERSION 3.16.3)

set(NAME mtg-stock-app)

project(
    mtg-stock-app
    DESCRIPTION "App to save and update your mtg stock into cardmarket"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Command to build the Rust project in the backend directory
# add_custom_command(
#     OUTPUT ${CMAKE_BINARY_DIR}/backend/target/release/librust_backend.so
#     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/backend
#     COMMAND cargo build --release
#     COMMENT "Building Rust backend"
# )

# Path to the compiled Rust library (adjust depending on your OS and library type)
if(APPLE)
    set(RUST_LIB_PATH ${CMAKE_SOURCE_DIR}/backend/target/release/librust_backend.dylib)
elseif(UNIX)
    set(RUST_LIB_PATH ${CMAKE_SOURCE_DIR}/backend/target/release/librust_backend.so)
elseif(WIN32)
    set(RUST_LIB_PATH ${CMAKE_SOURCE_DIR}/backend/target/release/rust_backend.dll)
endif()

# # Create a target for the Rust library so C++ can link against it
add_custom_target(rust_backend DEPENDS ${RUST_LIB_PATH})

add_subdirectory(vendor)

add_executable(${NAME} src/main.cpp src/application.cpp src/application.hpp src/window.hpp src/window.cpp
                        src/user.cpp src/user.hpp imgui-filebrowser/imfilebrowser.h dbg.h)

add_dependencies(mtg-stock-app rust_backend)                        

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(${NAME} PRIVATE cxx_std_20)
target_link_libraries(${NAME}
  PUBLIC SDL2::SDL2 imgui ${RUST_LIB_PATH})

# if(UNIX AND NOT APPLE)
#   target_link_libraries(mtg-stock-app PRIVATE dl pthread)
# endif()