load("@EmbeddedSystemsBuildScripts//:unity.bzl", "unity_test")

unity_test(
    file_name = "Mutex_Test.c",
    deps = [
        "//:Mutex",
    ],
)

unity_test(
    file_name = "PeriodicScheduler_Test.c",
    deps = [
        "//:PeriodicScheduler",
    ]
)

unity_test(
    file_name = "MultiReaderBuffer_Test.c",
    deps = [
        "//:MultiReaderBuffer",
    ]
)

unity_test(
    file_name = "BitManipulation_Test.c",
    deps = [
        "//:BitManipulation",
        "@CException",
    ]
)