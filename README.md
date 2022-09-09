# EEL 6908 - Fault Attacks in AI Models

## Tensorflow Framework

### Build C++ API

For debugging purposes. Below commands was modified from this [repo](https://github.com/rangsimanketkaew/tensorflow-cpp-api/blob/main/compile_tensorflow_cpp.md). Official docs on commands and options supported by Bazel, check [this](https://bazel.build/docs/user-manual). The `.bazelc` file from the [Tensorflow repo](https://github.com/tensorflow/tensorflow/blob/master/.bazelrc) also helped in determining default and supported commands and options.



```bash
export CC=gcc
export CXX=g++
bazelisk build --jobs=8 --local_ram_resources="HOST_RAM*.75" \
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

