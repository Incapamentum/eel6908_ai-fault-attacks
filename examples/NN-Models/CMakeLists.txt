cmake_minimum_required(VERSION 3.23)
project(NN_Models)

set(CMAKE_CXX_STANDARD 17)

add_executable(NN_Models main.cpp networks/net.h)

set(CMAKE_PREFIX_PATH /usr/local/libtorch/share/cmake/Torch)

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

target_link_libraries(NN_Models "${TORCH_LIBRARIES}")
