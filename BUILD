load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar", "pkg_deb")

filegroup(
    name = "UtilHeaders",
    srcs = glob(["Util/*.h"]),
)
    

cc_library(
    name = "Util",
    hdrs = [":UtilHeaders"],
    srcs = glob(["src/**/*.c", "src/**/*.h"]),
    visibility = ["//visibility:public"],
    deps = ["@CException"],
    linkstatic = True
)

cc_library(
    name = "BitManipulation",
    hdrs = ["Util/BitManipulation.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "Mutex",
    hdrs = [
        "Util/Atomic.h",
        "Util/Callback.h",
        "Util/Mutex.h",
    ],
    srcs = [
        "src/Mutex.c"
    ],
    visibility = ["//visibility:public"],
    deps = ["@CException"],
    linkstatic = True
    
)

cc_library(
    name = "PeriodicScheduler",
    hdrs = [
        "Util/PeriodicScheduler.h",
    ],
    srcs = [
        "src/PeriodicScheduler.c",
        "src/PeriodicSchedulerIntern.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":Debug",
        "@CException"
    ],
    linkstatic = True
)

cc_library(
    name = "Debug",
    hdrs = [
        "Util/Debug.h",
    ],
    visibility = ["//visibility:public"]
)

cc_library(
    name = "Callback",
    hdrs = [
        "Util/Callback.h",
    ],
    visibility = ["//visibility:public"]
)

cc_library(
    name = "MultiReaderBuffer",
    hdrs = [
        "Util/MultiReaderBuffer.h",
    ],
    srcs = [
        "src/MultiReaderBuffer.c",
    ],
    visibility = ["//visibility:public"],
    deps = ["@CException"],
    linkstatic = True
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

LibsList = [":MultiReaderBuffer", ":Callback", ":Debug", ":PeriodicScheduler", ":Util", ":BitManipulation", ":Mutex"]

pkg_tar(
    name = "pkgHeaders",
    srcs = [":UtilHeaders"],
    strip_prefix = ".",
    extension = "tar.gz",
    mode = "0644"
)

[pkg_tar(
    name = "pkg%s" % lib[1:],
    srcs = [lib],
    extension = "tar.gz",
    mode = "0644"
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
    deps = ["pkgHeaders", "pkgBuild"] + ["pkg%s" % lib[1:] for lib in LibsList],
    extension = "tar.gz",
    mode = "0644"
)