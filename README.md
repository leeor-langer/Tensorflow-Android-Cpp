# TensorFlow shared library

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

(2) Connect your phone. Push the binary to your phone with adb push
     (make the directory if required):
```
adb push /media/wld/HDD/tf_source/tensorflow/bazel-bin/tensorflow/tools/tfwld/tfwld /data/local/tmp
```

(3) Push the compute graph that you need to test. For example:
```
adb push /home/wld/Downloads/gesturesopt_embedding_0.pb /data/local/tmp
```
(4) Run the benchmark. For example:
```
adb shell /data/local/tmp/tfwld \
  --graph=/data/local/tmp/my_model_0.pb 
```

### On PC:
(1) build the library
```
bazel build -c opt --copt="-fPIC" --cxxopt='-std=c++11' tensorflow/tools/tfwld:libTensorflowInference.so --verbose_failures
```

(2) Run on your compute graph, similar to the Android case but without the need of adb shell.
For example:
```
bazel-bin/tensorflow/tools/tfwld/tfwld_inference --graph=/home/wld/Downloads/my_model_0.pb 

```

Note - This project is based on the Tensorflow benchmark tool.

