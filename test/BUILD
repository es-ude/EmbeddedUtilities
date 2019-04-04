load(
    "@UnityPlugin//Unity_rules:unity.bzl",
    "unity_test",
)

cexception_copts = [
    "-DCEXCEPTION_T=uint8_t",
    "-DCEXCEPTION_NONE=0x00",
]

unity_test(
    copts = cexception_copts,
    file_name = "Mutex_Test.c",
    deps = [
        "//:Mutex",
    ],
)

unity_test(
    copts = cexception_copts,
    file_name = "PeriodicScheduler_Test.c",
    deps = [
        "//:PeriodicScheduler",
    ]
)

unity_test(
    copts = cexception_copts,
    file_name = "MultiReaderBuffer_Test.c",
    deps = [
        "//:MultiReaderBuffer",
    ]
)