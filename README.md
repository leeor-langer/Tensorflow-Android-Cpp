# TensorFlow Inference (cross platform \ shared library)

## Description
A C++ library for running a Tensorflow compute graph (run directly, no need for JNI as in libtensorflow_inference.so),
both on desktop machines and on Android. Compilation works for Ubuntu 18.04.1 with Tensorflow release 1.9, Bazel 0.16.1.

## Instructions
1. Create the following directory: tf_source/tensorflow/tensorflow/tools/tfwld. Place all files (except precompiled .so file and WORKSPACE) inside. 
2. Place WORKSPACE file in tf_source/tensorflow. Note the NDK definitions (install location etc.) at the bottom of the file.
3. Make sure all installations are properly functioning (Tensorflow, Bazel, Clang, NDK).
4. Change path in BUILD file to the correct path of --version-script (this path points to the file named TensorflowInference.lds) 
5. Open command prompt from tf_source/tensorflow
6. Run command below

## Build

### On Android:

(0) Refer to https://github.com/tensorflow/tensorflow/tree/master/tensorflow/examples/android to edit the `WORKSPACE` to configure the android NDK/SDK.

(1) build command:
```
bazel build -c opt --copt="-fPIC" --cxxopt='-std=c++11' --crosstool_top=//external:android/crosstool --cpu=arm64-v8a --host_crosstool_top=@bazel_tools//tools/cpp:toolchain --config monolithic tensorflow/tools/tfwld:libTensorflowInference.so
```

### On PC:

```
bazel build -c opt --copt="-fPIC" --cxxopt='-std=c++11' tensorflow/tools/tfwld:libTensorflowInference.so --verbose_failures
```

Note - This project is based on the Tensorflow benchmark tool. Compiled library is also available for download (arm-v8a)

