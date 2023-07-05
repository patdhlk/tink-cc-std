"""
File with all 3rd party dependencies needed for the project

They are loaded but without loading the transitive dependencies, this is done in
a separated file extra_dependencies.bzl
"""

load("//third_party/rules_foreign_cc:load.bzl", "load_foreign_cc")
load("//third_party/wolfssl:load.bzl", "load_wolfssl")
load("//third_party/cifuzz:load.bzl", "load_cifuzz")


def load_third_party_libraries():
    """Load all third party dependencies"""
    load_foreign_cc()
    load_wolfssl()
    load_cifuzz()