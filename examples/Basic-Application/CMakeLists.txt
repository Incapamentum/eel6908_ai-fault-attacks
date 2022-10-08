cmake_minimum_required(VERSION 3.23)
project(Basic_Application)

set(CMAKE_CXX_STANDARD 17)

add_executable(Basic_Application main.cpp)

set(CMAKE_PREFIX_PATH /usr/local/libtorch/share/cmake/Torch)

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

target_link_libraries(Basic_Application "${TORCH_LIBRARIES}")