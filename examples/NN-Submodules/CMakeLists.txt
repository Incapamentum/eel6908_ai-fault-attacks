cmake_minimum_required(VERSION 3.23)
project(NN_Submodules)

set(CMAKE_CXX_STANDARD 17)

add_executable(NN_Submodules main.cpp networks/net.h)

set(CMAKE_PREFIX_PATH /usr/local/libtorch/share/cmake/Torch)

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

target_link_libraries(NN_Submodules "${TORCH_LIBRARIES}")