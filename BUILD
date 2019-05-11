load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_deb", "pkg_tar")

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

LibsList = [
    ":MultiReaderBuffer",
    ":Callback",
    ":Debug",
    ":PeriodicScheduler",
    ":Util",
    ":BitManipulation",
    ":Mutex",
]

pkg_tar(
    name = "pkgHeaders",
    srcs = [":UtilHeaders"],
    extension = "tar.gz",
    mode = "0644",
    strip_prefix = ".",
)

[pkg_tar(
    name = "pkg%s" % lib[1:],
    srcs = [lib],
    extension = "tar.gz",
    mode = "0644",
) for lib in LibsList]

pkg_tar(
    name = "pkgBuild",
    srcs = ["BUILD.tpl"],
    extension = "tar.gz",
    mode = "0644",
    remap_paths = {
        "BUILD.tpl": "BUILD",
    },
)

pkg_tar(
    name = "pkg",
    extension = "tar.gz",
    mode = "0644",
    deps = [
        "pkgHeaders",
        "pkgBuild",
    ] + ["pkg%s" % lib[1:] for lib in LibsList],
)
