# TensorFlow Inference (cross platform \ shared library)

## Description
A C++ library for running a Tensorflow compute graph,
both on desktop machines and on Android. Compilation works for Ubuntu 18.04.1 with Tensorflow release 1.9, Bazel 0.16.1.

## Instructions
1. Create the following directory: tf_source/tensorflow/tensorflow/tools/tfwld
2. Place WORKSPACE file in tf_source/tensorflow. Note the NDK definitions (install location etc.) at the bottom of the file.
3. Make sure all installations are properly functioning (Tensorflow, Bazel, gcc, NDK).
4. Change bath in BUILD file to the correct path of --version-script (this path point to the file named TensorflowInference.lds) 
5. Place WORKSPACE file in tf_source/tensorflow
6. Change directory to tf_source/tensorflow
7. Run commands below

## Build / Install / Run

### On Android:

(0) Refer to https://github.com/tensorflow/tensorflow/tree/master/tensorflow/examples/android to edit the `WORKSPACE` to configure the android NDK/SDK.

(1) build for your specific platform, e.g.:
```
bazel build -c opt --copt="-fPIC" --cxxopt='-std=c++11' --crosstool_top=//external:android/crosstool --cpu=arm64-v8a --host_crosstool_top=@bazel_tools//tools/cpp:toolchain --config monolithic tensorflow/tools/tfwld:libTensorflowInference.so
```

### On PC:

```
bazel build -c opt --copt="-fPIC" --cxxopt='-std=c++11' tensorflow/tools/tfwld:libTensorflowInference.so --verbose_failures
```

Note - This project is based on the Tensorflow benchmark tool. Compiled library is also available for download (arm-v8a)

