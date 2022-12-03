# PyTorch

PyTorch is an open-source machine learning framework that's built in C++ and uses Python as a frontend API endpoint. Versions for a C++/Java interface are also available. The C++ frontend is sometimes referred to as LibTorch.

PyTorch can either be downloaded and installed, or built from source. Detailed instructions on how to build from source can be found [here](pytorch_compilation.md).

## Codebase Structure

The official codebase can be found on [GitHub](https://github.com/pytorch/pytorch). This section describes some of the more "core" aspects of the backend and how to navigate them. A lot of detail and information was obtained from [ezyang's blog](http://blog.ezyang.com/2019/05/pytorch-internals/) on PyTorch internals, which is a useful read to get started on contributing towards the project or doing your own thing.

The following are the "core" directories that are responsible for a lot of the implementation details in PyTorch:

- `c10/` contains core lib files that work everywhere. This is where the core abstractions exist in.
  - `core/` contains not only the major abstractions used in all other platform implementations (i.e. CUDA and mobile), it also contains CPU-specific implementations, such as `CPUAllocator.cpp` and `CPUAllocator.h`.
- `aten/` is _the_ C++ tensor library, where the majority of different Tensor operations (i.e. _ops_) are implemented.
  - `src/ATen/` contains a lot of source files which contain the actual Tensor implementations. **NOTE**: `ATen` is short for A Tensor.
    - `core/` contains the core functionality of ATen.
      - For example, this is where `Tensor.h` and `Tensor.cpp` live in.
    - `native/` are modern implementations of ops. Contains a large number of directories for more specific ops, such as `cuda/` and `sparse/`.
- `torch/` is the main entry point for PyTorch, both for Python and C++ frontends.
  - `csrc/` contains all code concerned with Python integration.