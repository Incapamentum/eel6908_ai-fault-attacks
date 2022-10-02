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