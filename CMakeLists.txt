project(providentia)

cmake_minimum_required(VERSION 3.1)
find_package(LLVM REQUIRED CONFIG)

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

set(CMAKE_SHARED_LINKER_FLAGS "-ljsoncpp, -lboost") 
# sudo apt install libjsoncpp-dev
# sudo apt install libboost-dev 
add_subdirectory(musl-llvm-pass)
