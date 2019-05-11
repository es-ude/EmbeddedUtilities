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
    visibility = ["//visibility:public"],
    deps = ["@CException"],
)

cc_library(
    name = "MutexHdrsOnly",
    hdrs = [
        ":MutexHdrs",
    ],
    linkstatic = True,
    visibility = ["//visibility:public"],
)

cc_library(
    name = "PeriodicScheduler",
    srcs = ["libPeriodicScheduler.a"],
    hdrs = [
        ":PeriodicSchedulerHdrs",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@CException",
    ],
)

cc_library(
    name = "PeriodicSchedulerHdrsOnly",
    hdrs = [
        ":PeriodicSchedulerHdrs",
    ],
    linkstatic = True,
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
        "libMultiReaderBuffer.a",
    ],
    deps = [
        "@CException",
    ],
)

exports_files(
    srcs = glob(["Util/**/*.h"]),
    visibility = ["//visibility:public"],
)
