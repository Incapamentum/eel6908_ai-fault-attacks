# TensorFlow C++ API Compilation

The following are directions on installing the Tensorflow C++ API from source. This process can be very tricky, and I have documented the build process I have taken below. Some insight was gained by looking through different sources, such as the [official docs](https://www.tensorflow.org/install/source) and [third-party](https://github.com/rangsimanketkaew/tensorflow-cpp-api/blob/main/compile_tensorflow_cpp.md).

## Platform

OS: Ubuntu 20.04.5 LTS

CPU: AMD Ryzen 7 3700X

RAM: 32GB

## Dependencies

- Conda 4.14.0
- Python3* 3.9.13
  - Numpy 1.23.1
- GCC* 7.5.0
- Bazelisk* 3.7.2
- Protobul

* Versions of these tools will depend on the version of the Tensorflow C++ API that will be built. A list of supported tools can be found [here](https://www.tensorflow.org/install/source#tested_build_configurations).

## 1) Environment Setup

Followed [these](https://docs.anaconda.com/anaconda/install/linux/) installation instructions for Anaconda. Then, created the environment:

```bash
conda create -n tfcc
conda activate tfcc
conda update --all -y
conda install python==3.9.9 # Package is unavailable; using v3.8.10
conda install numpy
```

According to the official docs, there are additional package dependencies:

```bash
conda install wheel packaging requests opt_einsum
conda install -c conda-forge keras-preprocessing --no-deps
```

Seems like the `keras-preprocessing` package isn't located in any of the sources where Conda looks in. A work-around is by specifying the source. In this case, a package for `keras-preprocessing` can be found in `conda-forge`.

## 2) Install Bazel

I highly recommend building with Bazelisk as opposed to using Bazel. Big reason being is that Bazelisk automatically downloads the correct Bazel version needed to build Tensorflow. I recommended following [https://docs.bazel.build/versions/main/install-bazelisk.html](https://docs.bazel.build/versions/main/install-bazelisk.html) installation instructions.

Once Bazelisk is installed, I ended up creating a symbolic link in `usr/bin/` to associate `bazelisk` with `bazel` as follows:

```bash
which bazelisk # Double-check to ensure the install location of Bazelisk
sudo ln -s /usr/bin/bazelisk /usr/bin/bazel # Better way than using an alias
```

## 3) Configure the build

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

**NOTE**: in configuring the build, there are a lot of options presented. Be wary of what you're selecting. Check out the [https://www.tensorflow.org/install/source#expandable-1](https://www.tensorflow.org/install/source#expandable-1) if you'd like to see a sample configuration session.

## 4) Compile

The Tensorflow C++ API will be compiled to a shared library. It will be compiled using `bazelisk`:

```bash
export CC=gcc
export CXX=g++
bazel build --jobs=8 --local_ram_resources="HOST_RAM*.50" \
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

### Addendum

The above instructions were taken from a third-party repo. It helped as it provided a useful reference into what compile options should be pursued. The below is what ended up working for me in the end:

```bash
export CC=gcc-7
export CXX=g++-7
bazel build --jobs=8 --local_ram_resources="HOST_RAM*.50" \
	--cxxopt="-D_GLIBCXX_USE_CX11_ABI=0" \
    --strip=never --config=dbg \
    --config=noaws --config=nogcp --config=nohdfs --config=nonccl \
    --config=monolithic \
    //tensorflow:libtensorflow.so \
    //tensorflow:libtensorflow_cc.so \
    //tensorflow:libtensorflow_framework.so \
    //tensorflow:install_headers \
    //tensorflow/tools/pip_package:build_pip_package
```

The above builds the Tensorflow C++ API with debug symbols via the `--config=dbg` option. This was inferred from the official repo, in the contribution section for [debug builds](https://github.com/tensorflow/tensorflow/blob/master/CONTRIBUTING.md#debug-builds).

## 5) Sanity Check

Although this was presented as optional, for the sake of anyone's mental sanity, I highly recommend checking that the build was successful (even if it seems like it was):

```bash
bazel test --jobs=8 --cxxopt="-D_GLIBCXX_USE_CXX11_ABI=0" \
	//tensorflow/tools/lib_package:libtensorflow_test
```

**NOTE**: performing unit tests requires that the JRE/JDK Is installed.

## 6) Protobuf Check

1. Check the version of protobuf supported by TensorFlow. From the source directory:

```bash
bazel-bin/external/com_google_protobuf/protoc --version
```

2. Once the version is known, download the correct source code from its [release page](https://github.com/protocolbuffers/protobuf/tags).
3. Go ahead and untar the archive:

```bash
tar -xf protobuf-<VERSION_NUMBER>.tar.gz -C /PATH/TO/DUMP/TO
```

2. From source directory, compile and link:

```bash
./configure
make
make check
make install
```

## 7) Copy Required Files to a Single Path

At this point, both Protobuf and Tensorflow C++ API have been built. Now, we will copy the necessary files into a single path for C++ linkage purposes:

```bash
# Create directories
sudo mkdir /usr/local/tensorflow
sudo mkdir /usr/local/tensorflow/lib

# From tensorflow source directory:
sudo cp -r bazel-bin/tensorflow/include/ /usr/local/tensorflow/
sudo cp -r bazel-bin/tensorflow/*.so* /usr/local/tensorflow/lib

# From protobuf source directory:
sudo cp -r /PATH/TO/SOURCE/protobuf-<VERSION_NUMBER>/include/google/ /usr/local/tensorflow/include/
sudo cp -r /PATH/TO/SOURCE/protobuf-<VERSION_NUMBER>/lib/*.so* /usr/local/tensorflow/lib
```

