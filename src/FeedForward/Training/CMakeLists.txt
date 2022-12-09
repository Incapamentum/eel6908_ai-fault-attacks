cmake_minimum_required(VERSION 3.23)
project(Training)

set(CMAKE_CXX_STANDARD 17)

add_executable(Training training.cpp)
target_include_directories(Training PRIVATE ../)

set(CMAKE_PREFIX_PATH /usr/local/libtorch/share/cmake/Torch)

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

target_link_libraries(Training "${TORCH_LIBRARIES}")