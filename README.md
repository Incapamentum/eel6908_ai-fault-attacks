# EEL 6908 - Fault Attacks in AI Models

## Tensorflow Framework

Tensorflow (TF) is a free and open-source software library in developing ML and AI applications. It has a particular focus towards the training and inference of deep neural networks (DNNs).

### Compile Tensforflow C++ API from Source

The following are directions on installing the Tensorflow C++ API from source. This process can be very tricky, and I have documented the build process I have taken below. Some insight was gained by looking through different sources, such as the [official docs](https://www.tensorflow.org/install/source) and [third-party](https://github.com/rangsimanketkaew/tensorflow-cpp-api/blob/main/compile_tensorflow_cpp.md).

#### Platform

OS: Ubuntu 20.04.5 LTS

CPU: AMD Ryzen 7 3700X

RAM: 32GB

#### Dependencies

- Conda 4.14.0
- Python3* 3.8.10
  - Numpy 1.23.1
- GCC* 9.4.0
- Bazelisk* 5.0.0
- Protobul

* Versions of these tools will depend on the version of the Tensorflow C++ API that will be built. A list of supported tools can be found [here](https://www.tensorflow.org/install/source#tested_build_configurations).

#### 1) Environment Setup

Followed [these](https://docs.anaconda.com/anaconda/install/linux/) installation instructions for Anaconda. Then, created the environment:

```bash
conda create -n tfcc
conda activate tfcc
conda update --all -y
conda install python==3.9.9 # Package is unavailable; using v3.8.10
conda install numpy
```

According to the official docs, there are additional package dependencies (currently unsure if needed for C++ API):

```bash
conda install wheel packaging requests opt_einsum
conda install keras_preprocessing --no-deps # Package not available
```

#### 2) Install Bazel

I highly recommend building with Bazelisk as opposed to using Bazel. Big reason being is that Bazelisk automatically downloads the correct Bazel version needed to build Tensorflow. I recommended following [https://docs.bazel.build/versions/main/install-bazelisk.html](https://docs.bazel.build/versions/main/install-bazelisk.html) installation instructions.

#### 3) Configure the build

Download/clone the GitHub repo to system:

```bash
git clone https://github.com/tensorflow/tensorflow
cd tensorflow
git checkout r2.7
```

Configure the build:

```bash
./configure
```

According to the official docs, you may have to use `python configure.py` if using a virtual environment, as it prioritizes paths within the environment. I'm currently unsure whether this affects the Conda environment or not.

**NOTE**: in configuring the build, there are a lot of options presented. Be wary of what you're selecting.

#### 4) Compile

The Tensorflow C++ API will be compiled to a shared library. It will be compiled using `bazelisk`:

```bash
export CC=gcc
export CXX=g++
bazelisk build --jobs=8 --local_ram_resources="HOST_RAM*.50" \
	--cxxopt="-D_GLIBCXX_USE_CX11_ABI=0" \
	--strip=never --copt=-O -c dbg -c opt \ # Debug optimizations
	--copt=-mfma --copt=-msse4.1 \ # Include for instruction optimizations
    --copt=-msse4.2 --copt=-mfpmath=sse \ # Include for instruction optimizations
	--config=noaws --config=nogcp \ # Don't build for AWS/GCP platforms
	--config=nohdfs --config=nonccl \ # Don't build for HDFS/NCCL platforms
	--config=monolithic \ # Single (but large) shared library
	//tensorflow:libtensorflow.so \
	//tensorflow:libtensorflow_cc.so \
	//tensorflow:libtensorflow_framework.so \
	//tensorflow:install_headers \
	//tensorflow/tools/pip_package:build_pip_package
```

**NOTE**: I'm not always using the above options due to sometimes the build failing, and it is difficult to understand what causes the errors.
