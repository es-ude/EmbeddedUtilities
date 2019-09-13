load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_deb", "pkg_tar")

filegroup(
    name = "UtilHeaders",
    srcs = glob(["EmbeddedUtilities/*.h"]),
)

cc_library(
    name = "EmbeddedUtilities",
    srcs = glob([
        "src/**/*.c",
        "src/**/*.h",
    ]),
    hdrs = [":UtilHeaders"],
    linkstatic = True,
    visibility = ["//visibility:public"],
    deps = ["@CException"],
)

cc_library(
    name = "UtilHdrsOnly",
    hdrs = [":UtilHeaders"],
    linkstatic = True,
    visibility = ["//visibility:public"],
)

cc_library(
    name = "BitManipulation",
    hdrs = ["EmbeddedUtilities/BitManipulation.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "Mutex",
    srcs = [
        "src/Mutex.c",
    ],
    hdrs = [
        "EmbeddedUtilities/Atomic.h",
        "EmbeddedUtilities/Callback.h",
        "EmbeddedUtilities/Mutex.h",
    ],
    linkstatic = True,
    visibility = ["//visibility:public"],
    deps = ["@CException"],
)

cc_library(
    name = "MutexHdrsOnly",
    hdrs = [
        "EmbeddedUtilities/Atomic.h",
        "EmbeddedUtilities/Callback.h",
        "EmbeddedUtilities/Mutex.h",
    ],
    linkstatic = True,
    visibility = ["//visibility:public"],
)

cc_library(
    name = "PeriodicScheduler",
    srcs = [
        "src/PeriodicScheduler.c",
        "src/PeriodicSchedulerIntern.h",
    ],
    hdrs = [
        "EmbeddedUtilities/PeriodicScheduler.h",
    ],
    linkstatic = True,
    visibility = ["//visibility:public"],
    deps = [
        ":Debug",
        "@CException",
    ],
)

cc_library(
    name = "PeriodicSchedulerHdrsOnly",
    hdrs = [
        "EmbeddedUtilities/PeriodicScheduler.h",
    ],
    linkstatic = True,
    visibility = ["//visibility:public"],
)

cc_library(
    name = "Debug",
    hdrs = [
        "EmbeddedUtilities/Debug.h",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "Callback",
    hdrs = [
        "EmbeddedUtilities/Callback.h",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "MultiReaderBuffer",
    srcs = [
        "src/MultiReaderBuffer.c",
    ],
    hdrs = [
        "EmbeddedUtilities/MultiReaderBuffer.h",
    ],
    linkstatic = True,
    visibility = ["//visibility:public"],
    deps = ["@CException"],
)

cc_library(
    name = "MultiReaderBufferHdrsOnly",
    linkstatic = True,
    visibility = ["//visibility:public"],
)

"""
Export public header files to make
them available for mocking from inside
other projects.
"""

exports_files(
    srcs = glob(["EmbeddedUtilities/**/*.h"]),
    visibility = ["//visibility:public"],
)

pkg_tar(
    name = "pkg",
    srcs = glob([
        "EmbeddedUtilities/*.h",
        "src/*.c",
        "src/*.h",
    ]) + ["BUILD"],
    extension = "tar.gz",
    mode = "0644",
    strip_prefix = ".",
)
