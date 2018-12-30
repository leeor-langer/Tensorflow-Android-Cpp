# Description:
#   Benchmark utility that can run on desktop and Android.

package(default_visibility = ["//visibility:public"])

licenses(["notice"])  # Apache 2.0

load(
    "//tensorflow:tensorflow.bzl",
    "tf_copts",
    "tf_cc_test",
    "tf_cc_binary",
)

#exports_files(["LICENSE"])

cc_library(
    name = "Logging",
    srcs = ["Logging.cpp"],
    hdrs = ["Logging.h"],
)

cc_library(
    name = "TensorflowInference_lib",
    testonly = 1,
    srcs = [
        "TensorflowInference.cc",
    ],
    hdrs = [
        "TensorflowInference.h",
    ],
    copts = tf_copts(),
    visibility = ["//visibility:public"],
    deps = select({
        "//tensorflow:android": [
            "//tensorflow/core:android_tensorflow_lib",
            "//tensorflow/core:android_tensorflow_test_lib",
            ":Logging",
        ],
        "//conditions:default": [
            "//tensorflow/core:core_cpu",
            "//tensorflow/core:lib",
            "//tensorflow/core:framework",
            "//tensorflow/core:framework_internal",
            "//tensorflow/core:framework_lite",
            "//tensorflow/core:protos_all_cc",
            "//tensorflow/core:tensorflow",
            "//tensorflow/core:test",
        ],
    }),
)

# This binary may be built for either desktop or Android.
# A typical Android build command will look like the following:
# bazel build tensorflow/core:android_tensorflow_lib \
# --crosstool_top=//external:android/crosstool \
# --cpu=armeabi-v7a \
# --host_crosstool_top=@bazel_tools//tools/cpp:toolchain
# --config monolithic
# linkshared = 1, 
# visibility = ["//visibility:public"],
#hdrs = ["tfwld_inference.h"],
#"-s",
#"-s",
#"-Wl,--exclude-libs,ALL",  # Exclude syms in all libs from auto export
#            "-s",
#            "-Wl,--exclude-libs,ALL",  # Exclude syms in all libs from auto export
# "-Xlinker,--enable-auto-import",
#  "-Wl,--exclude-libs,ALL",  # Exclude syms in all libs from auto export
# "-s",
#"-Wl,--exclude-libs,ALL",  # Exclude syms in all libs from auto export
#"-Wl,--version-script=tfwld_inference.lds",
cc_binary(
    name = "libTensorflowInference.so",
    testonly = 1,
    srcs = ["TensorflowInference.cc", "Logging.cpp", "Logging.h"],
    copts = tf_copts(),
    linkopts = select({
        "//tensorflow:android": [
            "-shared",
            "-landroid",
            "-latomic",
            "-ljnigraphics",
            "-llog",
            "-lm",
            "-z defs",
	    "-Wl,--allow-multiple-definition",
            "-Wl,--version-script=/media/wld/HDD/tf_source/tensorflow/tensorflow/tools/tfwld/TensorflowInference.lds",
            "-s",
        ],
        "//conditions:default": [],
    }),
    linkstatic = 1,
    linkshared = 1,
    visibility=["//visibility:private"],
    deps = [":TensorflowInference_lib", ":Logging"],
)


