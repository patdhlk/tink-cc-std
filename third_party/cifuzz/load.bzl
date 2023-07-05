"""
Dependency 
"""
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_cifuzz():
    # It is not clear in Bazel what is the best practice for using http_archive.
    # If you call http_archive without any kind of check, you could call it two times
    # with the same name and different parameters and you would not get any warning/error.
    #
    # One option is to check if it is already available in the existing_rules and only
    # call http_archive if it is not present. In the else you could display a message in
    # case that was already present but in reality you would only want a warning/error if was
    # already called with different parameters (different library version for example).
    #
    # Another option is to wrap the http_archive in a maybe call but this will also not display
    # a warning. It behaves like the if check with the advantage that the name has not to be
    # repeated

    maybe(
        http_archive,
        name = "rules_fuzzing",
        sha256 = "4beab98d88e4bf2d04da0412d413a1364f95e5eb88963e15e603bee1410fcedf",
        strip_prefix = "rules_fuzzing-ca617e846d0f92e00a903903b0554ea9142e1132",
        urls = ["https://github.com/CodeIntelligenceTesting/rules_fuzzing/archive/ca617e846d0f92e00a903903b0554ea9142e1132.tar.gz"],
    )

    maybe(
        git_repository,
        name = "cifuzz",
        commit = "b013aa0f90fe8ac60adfc6d9640a9cfa451dda9e",
        remote = "https://github.com/CodeIntelligenceTesting/cifuzz-bazel",
    )