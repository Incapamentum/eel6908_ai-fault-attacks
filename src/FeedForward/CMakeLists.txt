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

# NOTE: follow the below portions for the Inference subproject

option(BUILD_TRAINING "Enable building Training" ON)
option(BUILD_INFERENCE "Enable building Inference" ON)

if (BUILD_TRAINING)
    add_subdirectory(Training)
endif()
if (BUILD_INFERENCE)
    add_subdirectory(Inference)
endif()