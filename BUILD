cc_library(
    name = "Util",
    hdrs = glob(["Util/*.h"]),
    srcs = glob(["src/**/*.c"]),
    visibility = ["//visibility:public"],
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
    deps = ["@CException"]
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
    ],
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