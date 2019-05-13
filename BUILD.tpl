filegroup(
    name = "MutexHdrs",
    srcs = [
        "Util/Atomic.h",
        "Util/Callback.h",
        "Util/Mutex.h",
    ],
)

filegroup(
    name = "BitManipulationHdrs",
    srcs = [
        "Util/BitManipulation.h",
    ],
)

filegroup(
    name = "PeriodicSchedulerHdrs",
    srcs = [
        "Util/PeriodicScheduler.h",
    ],
)

filegroup(
    name = "MultiReaderBufferHdrs",
    srcs = [
        "Util/MultiReaderBuffer.h",
    ],
)

filegroup(
    name = "UtilHdrs",
    srcs = [
        ":BitManipulationHdrs",
        ":MultiReaderBufferHdrs",
        ":MutexHdrs",
        ":PeriodicSchedulerHdrs",
    ],
)

cc_library(
    name = "Util",
    hdrs = [":UtilHeaders"],
    srcs = glob(["src/**/*.c", "src/**/*.h"]),
    srcs = ["libUtil.a"],
    hdrs = [":UtilHdrs"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "UtilHdrOnly",
    hdrs = [":UtilHdrs"],
    linkstatic = True,
    visibility = ["//visibility:public"],
)

cc_library(
    name = "BitManipulation",
    hdrs = [":BitManipulationHdrs"],
    linkstatic = True,
    visibility = ["//visibility:public"],
)

cc_library(
    name = "Mutex",
    srcs = ["libMutex.a"],
    hdrs = [
        ":MutexHdrs",
    ],
    srcs = [
        "src/Mutex.c"
        ],
    visibility = ["//visibility:public"],
    deps = ["@CException"],
)

cc_library(
    name = "MutexHdrsOnly",
    hdrs = [
        ":MutexHdrs",
    ],
    srcs = [
        "src/PeriodicScheduler.c",
        "src/PeriodicSchedulerIntern.h",
    ],    visibility = ["//visibility:public"],
    linkstatic = True,
    visibility = ["//visibility:public"],
)

cc_library(
    name = "PeriodicSchedulerHdrsOnly",
#    srcs = ["libPeriodicScheduler.a"],
    hdrs = [
        ":PeriodicSchedulerHdrs",
    ],
    linkstatic = True,
    visibility = ["//visibility:public"],
#    deps = ["@CException"],
)

cc_library(
    name = "Debug",
    hdrs = [
        "Util/Debug.h",
    ],
    linkstatic = True,
    visibility = ["//visibility:public"],
)

cc_library(
    name = "Callback",
    hdrs = [
        "Util/Callback.h",
    ],
    linkstatic = True,
    visibility = ["//visibility:public"],
)

cc_library(
    name = "MultiReaderBuffer",
    srcs = [
        "src/MultiReaderBuffer.c",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@CException",
    ],
)

exports_files(
    srcs = glob(["Util/**/*.h"]),
    visibility = ["//visibility:public"],
)
