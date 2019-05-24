load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_deb", "pkg_tar")
load("@AvrToolchain//:helpers.bzl", "mcu_avr_gcc_flag")

filegroup(
    name = "UtilHeaders",
    srcs = glob(["Util/*.h"]),
)

cc_library(
    name = "Util",
    srcs = glob([
        "src/**/*.c",
        "src/**/*.h",
    ]),
    copts = mcu_avr_gcc_flag(),
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
    hdrs = ["Util/BitManipulation.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "Mutex",
    srcs = [
        "src/Mutex.c",
    ],
    copts = mcu_avr_gcc_flag(),
    hdrs = [
        "Util/Atomic.h",
        "Util/Callback.h",
        "Util/Mutex.h",
    ],
    linkstatic = True,
    visibility = ["//visibility:public"],
    deps = ["@CException"],
)

cc_library(
    name = "MutexHdrsOnly",
    hdrs = [
        "Util/Atomic.h",
        "Util/Callback.h",
        "Util/Mutex.h",
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
    copts = mcu_avr_gcc_flag(),
    hdrs = [
        "Util/PeriodicScheduler.h",
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
        "Util/PeriodicScheduler.h",
    ],
    copts = mcu_avr_gcc_flag(),
    linkstatic = True,
    visibility = ["//visibility:public"],
)

cc_library(
    name = "Debug",
    hdrs = [
        "Util/Debug.h",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "Callback",
    hdrs = [
        "Util/Callback.h",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "MultiReaderBuffer",
    srcs = [
        "src/MultiReaderBuffer.c",
    ],
    hdrs = [
        "Util/MultiReaderBuffer.h",
    ],
    copts = mcu_avr_gcc_flag(),
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
    srcs = glob(["Util/**/*.h"]),
    visibility = ["//visibility:public"],
)

pkg_tar(
    name = "pkg",
    srcs = [
        "src/MultiReaderBuffer.c",
        "src/Mutex.c",
        "src/PeriodicScheduler.c",
        "src/PeriodicSchedulerIntern.h",
        "Util/Atomic.h",
        "Util/BitManipulation.h",
        "Util/Callback.h",
        "Util/Debug.h",
        "Util/MultiReaderBuffer.h",
        "Util/Mutex.h",
        "Util/PeriodicScheduler.h",
        "BUILD",
    ],
    extension = "tar.gz",
    mode = "0644",
    strip_prefix = ".",
)