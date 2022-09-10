# EEL 6908 - Fault Attacks in AI Models

## Tensorflow Framework

Tensorflow (TF) is a free and open-source software library in developing ML and AI applications. It has a particular focus towards the training and inference of deep neural networks (DNNs).

### Compile Tensforflow C++ API from Source

#### Dependencies

- Conda 4.14.0
- Python3* 3.10.4
  - Numpy 1.23.1
- GCC* 9.4.0
- Bazelisk* 5.0.0
- Protobul

* Versions of these tools will depend on the version of the Tensorflow C++ API that will be built. A list of supported tools can be found [here](https://www.tensorflow.org/install/source#tested_build_configurations).

#### Configure the build

Currently don't know what the below actually does vs `./configure` in the root of the workspace.

```bash
python configure.py
```

#### Compile & Install Libraries

For debugging purposes. Below commands was modified from this [repo](https://github.com/rangsimanketkaew/tensorflow-cpp-api/blob/main/compile_tensorflow_cpp.md). Official docs on commands and options supported by Bazel, check [this](https://bazel.build/docs/user-manual). The `.bazelc` file from the [Tensorflow repo](https://github.com/tensorflow/tensorflow/blob/master/.bazelrc) also helped in determining default and supported commands and options.



```bash
export CC=gcc
export CXX=g++
bazelisk build --jobs=8 --local_ram_resources="HOST_RAM*.50" \
	--cxxopt="-D_GLIBCXX_USE_CX11_ABI=0" --strip=never -c dbg \
	--copt=-mfma --copt=-msse4.1 --copt=-msse4.2 --copt=-mfpmath=sse \
	--config=noaws --config=nogcp --config=nohdfs --config=nonccl \
	--config=monolithic \
	//tensorflow:libtensorflow.so \
	//tensorflow:libtensorflow_cc.so \
	//tensorflow:libtensorflow_framework.so \
	//tensorflow:install_headers \
	//tensorflow/tools/pip_package:build_pip_package
```

