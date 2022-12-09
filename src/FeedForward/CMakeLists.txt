cmake_minimum_required(VERSION 3.23)
project(FeedForward)

set(CMAKE_CXX_STANDARD 17)

# Download MNIST Images
option(DOWNLOAD_MNIST "Download the MNIST dataset from the internet" ON)
if (DOWNLOAD_MNIST)
    message(STATUS "Downloading MNIST dataset")
    execute_process(
            COMMAND python ${CMAKE_CURRENT_LIST_DIR}/tools/download_mnist.py
            -d ${CMAKE_BINARY_DIR}/data
            ERROR_VARIABLE DOWNLOAD_ERROR)
    if (DOWNLOAD_ERROR)
        message(FATAL_ERROR "Error downloading MNIST dataset: ${DOWNLOAD_ERROR}")
    endif()
endif()

# Executables
add_executable(Training Training/training.cpp net/net.h)
add_executable(Inference Inference/inference.cpp net/net.h)

set(CMAKE_PREFIX_PATH /usr/local/libtorch/share/cmake/Torch)

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

target_link_libraries(Training "${TORCH_LIBRARIES}")
target_link_libraries(Inference "${TORCH_LIBRARIES}")
