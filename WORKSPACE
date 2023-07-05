workspace(name = "tink_cc")

# Use this repository if you want to build the FIPS module for BoringSSL
# local_repository(
#   name = "boringssl",
#   path = "third_party/boringssl_fips/",
# )

load("@tink_cc//:tink_cc_deps.bzl", "tink_cc_deps")
tink_cc_deps()

load("@tink_cc//:tink_cc_deps_init.bzl", "tink_cc_deps_init")
tink_cc_deps_init()

load("//third_party:third_party.bzl", "load_third_party_libraries")
load_third_party_libraries()

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")
rules_foreign_cc_dependencies()

load("@rules_fuzzing//fuzzing:repositories.bzl", "rules_fuzzing_dependencies")
rules_fuzzing_dependencies()

load("@rules_fuzzing//fuzzing:init.bzl", "rules_fuzzing_init")
rules_fuzzing_init()

