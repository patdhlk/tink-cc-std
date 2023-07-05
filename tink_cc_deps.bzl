"""Dependencies of C++ Tink."""

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def tink_cc_deps():
    """Loads dependencies of C++ Tink."""

    # Basic rules we need to add to bazel.
    if not native.existing_rule("bazel_skylib"):
        # Release from 2022-09-01: https://github.com/bazelbuild/bazel-skylib/releases/tag/1.3.0
        http_archive(
            name = "bazel_skylib",
            urls = [
                "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.3.0/bazel-skylib-1.3.0.tar.gz",
                "https://github.com/bazelbuild/bazel-skylib/releases/download/1.3.0/bazel-skylib-1.3.0.tar.gz",
            ],
            sha256 = "74d544d96f4a5bb630d465ca8bbcfe231e3594e5aae57e1edbf17a6eb3ca2506",
        )

    # -------------------------------------------------------------------------
    # Protobuf.
    # -------------------------------------------------------------------------
    # proto_library, cc_proto_library and java_proto_library rules implicitly
    # depend respectively on:
    #   * @com_google_protobuf//:proto
    #   * @com_google_protobuf//:cc_toolchain
    #   * @com_google_protobuf//:java_toolchain
    # This statement defines the @com_google_protobuf repo.
    if not native.existing_rule("com_google_protobuf"):
        # Release X.21.9 from 2022-10-26.
        http_archive(
            name = "com_google_protobuf",
            strip_prefix = "protobuf-21.9",
            urls = ["https://github.com/protocolbuffers/protobuf/archive/refs/tags/v21.9.zip"],
            sha256 = "5babb8571f1cceafe0c18e13ddb3be556e87e12ceea3463d6b0d0064e6cc1ac3",
        )

    # -------------------------------------------------------------------------
    # BoringSSL.
    # -------------------------------------------------------------------------
    if not native.existing_rule("boringssl"):
        # Commit from 2023-02-15.
        http_archive(
            name = "boringssl",
            strip_prefix = "boringssl-5c22014ca513807ed03c657e8ede076164663979",
            url = "https://github.com/google/boringssl/archive/5c22014ca513807ed03c657e8ede076164663979.zip",
            sha256 = "863fc670c456f30923740c1639305132fdfb9d1b25ba385a67ae3862ef12a8af",
        )

    # -------------------------------------------------------------------------
    # GoogleTest/GoogleMock.
    # -------------------------------------------------------------------------
    if not native.existing_rule("com_google_googletest"):
        http_archive(
            name = "com_google_googletest",  # 2023-02-28T13:15:29Z
            sha256 = "82ad62a4e26c199de52a707778334e80f6b195dd298d48d520d8507d2bcb88c4",
            strip_prefix = "googletest-2d4f208765af7fa376b878860a7677ecc0bc390a",
            # Keep this URL in sync with ABSL_GOOGLETEST_COMMIT in ci/cmake_common.sh.
            urls = ["https://github.com/google/googletest/archive/2d4f208765af7fa376b878860a7677ecc0bc390a.zip"],
        )

    # -------------------------------------------------------------------------
    # Wycheproof (depends on Rapidjson).
    # -------------------------------------------------------------------------
    if not native.existing_rule("rapidjson"):
        # Release from 2016-08-25 (still the latest release as of 2022-05-05).
        http_archive(
            name = "rapidjson",
            url = "https://github.com/Tencent/rapidjson/archive/v1.1.0.tar.gz",
            sha256 = "bf7ced29704a1e696fbccf2a2b4ea068e7774fa37f6d7dd4039d0787f8bed98e",
            strip_prefix = "rapidjson-1.1.0",
            build_file = "@tink_cc//:third_party/rapidjson.BUILD.bazel",
        )
    if not native.existing_rule("wycheproof"):
        # Commit from 2019-12-17.
        http_archive(
            name = "wycheproof",
            strip_prefix = "wycheproof-d8ed1ba95ac4c551db67f410c06131c3bc00a97c",
            url = "https://github.com/google/wycheproof/archive/d8ed1ba95ac4c551db67f410c06131c3bc00a97c.zip",
            sha256 = "eb1d558071acf1aa6d677d7f1cabec2328d1cf8381496c17185bd92b52ce7545",
        )

    # abseil dependencies
    # RE2 (the regular expression library used by GoogleTest)
    http_archive(
        name = "com_googlesource_code_re2",
        sha256 = "1726508efc93a50854c92e3f7ac66eb28f0e57652e413f11d7c1e28f97d997ba",
        strip_prefix = "re2-03da4fc0857c285e3a26782f6bc8931c4c950df4",
        urls = ["https://github.com/google/re2/archive/03da4fc0857c285e3a26782f6bc8931c4c950df4.zip"],  # 2023-06-01
    )

    # Google benchmark.
    http_archive(
        name = "com_github_google_benchmark",  # 2023-01-10T16:48:17Z
        sha256 = "ede6830512f21490eeea1f238f083702eb178890820c14451c1c3d69fd375b19",
        strip_prefix = "benchmark-a3235d7b69c84e8c9ff8722a22b8ac5e1bc716a6",
        urls = ["https://github.com/google/benchmark/archive/a3235d7b69c84e8c9ff8722a22b8ac5e1bc716a6.zip"],
    )

    # Bazel Skylib.
    http_archive(
        name = "bazel_skylib",  # 2022-11-16T18:29:32Z
        sha256 = "a22290c26d29d3ecca286466f7f295ac6cbe32c0a9da3a91176a90e0725e3649",
        strip_prefix = "bazel-skylib-5bfcb1a684550626ce138fe0fe8f5f702b3764c3",
        urls = ["https://github.com/bazelbuild/bazel-skylib/archive/5bfcb1a684550626ce138fe0fe8f5f702b3764c3.zip"],
    )

    # Bazel platform rules.
    http_archive(
        name = "platforms",  # 2022-11-09T19:18:22Z
        sha256 = "b4a3b45dc4202e2b3e34e3bc49d2b5b37295fc23ea58d88fb9e01f3642ad9b55",
        strip_prefix = "platforms-3fbc687756043fb58a407c2ea8c944bc2fe1d922",
        urls = ["https://github.com/bazelbuild/platforms/archive/3fbc687756043fb58a407c2ea8c944bc2fe1d922.zip"],
    )

    http_archive(
        name = "rules_foreign_cc",
        strip_prefix = "rules_foreign_cc-main",
        url = "https://github.com/bazelbuild/rules_foreign_cc/archive/main.zip",
    )


