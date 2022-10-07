# PyTorch API Compilation - Debugging

The following are directions on installing the PyTorch API from source for C++ debugging purposes. This should be a straightforward method of building the API.

Please note that this is a Python frontend with a C/C++ backend. These instructions mostly follow from the [official docs](https://github.com/pytorch/pytorch#installation), but with adjustments done to take into account my own particular use-case.

## Platform

OS: Ubuntu 20.04.5 LTS

CPU: AMD Ryzen 7 3700X

RAM: 32GB

## Dependencies

For installing from source, the following will be required:

- Python 3.7 or later (for Linux, Python 3.7.6+ or 3.8.1+ is needed)
- A C++14 compatible compiler

On my own system, I am using the following:

- Python 3.8.13 (via a Conda environment)
- GCC 9.4.0

## Installation

The following are sets of commands to setup a proper environment before building.

### 1) Environment Setup

Followed [these](https://docs.anaconda.com/anaconda/install/linux/) installation instructions for Anaconda. 

First, create and activate the environment:

```bash
conda create --name pytorch python=3.8
conda activate pytorch
```

Install additional dependencies:

```bash
conda install astunparse numpy ninja pyyaml setuptools cmake cffi typing_extensions future six requests dataclasses
conda install mkl mkl-include
# CUDA only: Add LAPACK support for the GPU if needed
conda install -c pytorch magma-cuda110  # or the magma-cuda* that matches your CUDA version from https://anaconda.org/pytorch/repo
```



### 2) Get PyTorch Source

```bash
git clone --recursive https://github.com/pytorch/pytorch
cd pytorch
# if you are updating an existing checkout
git submodule sync
git submodule update --init --recursive --jobs 0
```

### 3) Install PyTorch for Debugging

Building PyTorch is straightforward, as the following can be run:

```bash
python setup.py install
```

However, as the intent is to build for debugging, a different command will be used. Before that, some environmental variables may have to be exported *before* building happens:

```bash
export CMAKE_PREFIX_PATH=${CONDA_PREFIX:-"$(dirname $(which conda))/../"}
export USE_CUDA=0 # Disables CUDA support
export USE_ROCM=0 # Disables ROCm support
```

Additionally, other potentially useful environment variables may be found in `setup.py`, which will be further discussed.

#### C++ Development

The official docs for contributing to PyTorch has a section on [tips for C++ development](https://github.com/pytorch/pytorch/blob/master/CONTRIBUTING.md#c-development-tips), which mention a few important things to keep in mind. The first is to build only what's needed. The second is on advice on how to make rebuilds in the absence of changes go faster.

Of interest are in the usage of additional environment variables. For example, prepending `DEBUG=1` to the build command will enable debug information by compiling the C++ source files with the `-O0 -g` flags.

Therefore, the command to build PyTorch for C++ development is the following:

```bash
DEBUG=1 python setup.py build develop
```

#### C++ Stack traces

Per [Wikipedia](https://en.wikipedia.org/wiki/Stack_trace), a "stack trace is a report of the active stack frames at a certain point in time during the execution of a program". It allows for tracking the sequence of nested functions called - up to the point where the stack trace is generated. In a post-mortem scenario this extends up to the function where the failure occurred (but was not necessarily caused).

To enable C++ stack traces, prepend `TORCH_SHOW_CPP_STACKTRACES=1` to the build command.

## GDB Integration

According to the [official docs](https://github.com/pytorch/pytorch/blob/master/CONTRIBUTING.md#gdb-integration), there is GDB support in debugging PyTorch. in the `pytorch/tools/gdb` path there is a `pytorch-gdb.py` script that introduces some PyTorch-specific commands which can be used from the GDB prompt. In particular, `torch-tensor-repr` prints a human-readable representation of an at::Tensor object.

## LibTorch Compilation

LibTorch is the C++ frontend for PyTorch, as the core does not depend on Python. It is possible to [build from source](https://github.com/pytorch/pytorch/blob/master/docs/libtorch.rst) using a provided Python script, but the developers do mention that work is being done towards eliminating that build process in favor of a unified `cmake` build.

A set of [third-party instructions](https://github.com/shanemcandrewai/pytorch-setup) were found that may prove to be helpful, but note that their last update was in Nov 2020.

### Build using Python [POSSIBLY NON-FUNCTIONAL]

This approach does not rely on the `build_libtorch.py` script, which is bound to be removed. The normal way of building PyTorch, via `setup.py`, can be used to build LibTorch and copy the built C++ libraries to whichever location. For example:

```bash
cd <pytorch_root>
python setup.py build
```

Thus, an approach following previously outlined steps may be useful:

```bash
export CMAKE_PREFIX_PATH=${CONDA_PREFIX:-"$(dirname $(which conda))/../"}
export DEBUG=1
export BUILD_BINARY=1
export BUILD_CAFFE2=0
export BUILD_CAFFE2_OPS=0 # It's possible this option might be default if Caffe2 won't be built
export BUILD_TEST=0
export BUILD_PYTHON=0
export USE_CUDA=0
export USE_CUDNN=0
export USE_DISTRIBUTED=0
export USE_FBGEMM=0
export USE_MKLDNN=0
export USE_NNPACK=0
export USE_NUMPY=0
export USE_QNNPACK=0
export USE_ROCM=0
export USE_XNNPACK=0
python setup.py build
```

### Build using CMake [FUNCTIONAL AS OF OCT 6TH, 2022]

LibTorch can also be built directly using CMake. The following is a modified example taken from [official docs](https://github.com/shanemcandrewai/pytorch-setup) that uses this build process.

First, clone the PyTorch repo and create a build directory:

```bash
git clone --recursive https://github.com/pytorch/pytorch # Ensure this is the most up-to-date clone command
mkdir build_libtorch
cd build_libtorch
```

Then, run `cmake` with the following build options:

```bash
cmake -DBUILD_SHARED_LIBS:BOOL=ON -DBUILD_PYTHON=OFF -DBUILD_TEST=OFF \
-DTORCH_SHOW_CPP_STACKTRACES=ON -DUSE_CUDA=OFF -DUSE_DISTRIBUTED=OFF -DUSE_FBGEMM=OFF \
-DUSE_MAGMA=OFF -DUSE_MKLDNN=OFF -DUSE_NNPACK=OFF -DUSE_NUMA=OFF -DUSE_NUMPY=OFF \
-DUSE_OPENCV=0 -DUSE_OPENMP=OFF -DUSE_QNNPACK=OFF -DUSE_ROCM=OFF -DUSE_XNNPACK=OFF \
-DCMAKE_BUILD_TYPE:STRING=Debug \
-DPYTHON_EXECUTABLE:PATH=<path_to_python> \
-DCMAKE_INSTALL_PREFIX:PATH=<target_install_path> \
<path_to_pytorch_source>
```

**NOTE**: Other build options may be used in a similar use-case as the ones listed above.

Finally, build and install:

```bash
cmake --build . --target install
```

Once that's done, the required files will be copied to a single path for easy lookup:

```bash
```

