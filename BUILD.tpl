filegroup(
    name = "UtilHeaders",
    srcs = glob(["Util/*.h"]),
)
    
cc_library(
    name = "Util",
    hdrs = [":UtilHeaders"],
    srcs = glob(["src/**/*.c", "src/**/*.h"]),
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
    ],    visibility = ["//visibility:public"],
    deps = [
        ":Debug",
        "@CException"
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
    srcs = [
        "src/MultiReaderBuffer.c",
    ],
)

exports_files(
    srcs = glob(["Util/**/*.h"]),
    visibility = ["//visibility:public"],
)
