// Copyright 2023 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
////////////////////////////////////////////////////////////////////////////////

#include "tink/config/internal/global_registry.h"

#include "tink/absl/log/check.h"
#include "tink/internal/key_gen_configuration_impl.h"
#include "tink/key_gen_configuration.h"

namespace crypto {
namespace tink {
namespace internal {

const KeyGenConfiguration& KeyGenConfigGlobalRegistry() {
  static const KeyGenConfiguration* instance = [] {
    static KeyGenConfiguration* config = new KeyGenConfiguration();
    CHECK_OK(KeyGenConfigurationImpl::SetGlobalRegistryMode(*config));
    return config;
  }();
  return *instance;
}

}  // namespace internal
}  // namespace tink
}  // namespace crypto
