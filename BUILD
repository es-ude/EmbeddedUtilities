load("@AVR_Toolchain//:helpers.bzl", "default_embedded_lib")

default_embedded_lib(
    name = "Util",
    hdrs = glob(["Util/*.h"]),
    srcs = glob(["src/**/*.c"]),
    visibility = ["//visibility:public"],
)

default_embedded_lib(
    name = "BitManipulation",
    hdrs = ["Util/BitManipulation.h"],
    visibility = ["//visibility:public"],
)

default_embedded_lib(
    name = "Mutex",
    hdrs = [
        "Util/Atomic.h",
        "Util/Callback.h",
        "Util/Mutex.h",
    ],
    srcs = [
        "src/Mutex.c"
    ],
    visibility = ["//visibility:public"]
)

default_embedded_lib(
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
    ],
)

default_embedded_lib(
    name = "Debug",
    hdrs = [
        "Util/Debug.h",
    ],
    visibility = ["//visibility:public"]
)

default_embedded_lib(
    name = "Callback",
    hdrs = [
        "Util/Callback.h",
    ],
    visibility = ["//visibility:public"]
)

default_embedded_lib(
    name = "MultiReaderBuffer",
    hdrs = [
        "Util/MultiReaderBuffer.h",
    ],
    srcs = [
        "src/MultiReaderBuffer.c",
    ],
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