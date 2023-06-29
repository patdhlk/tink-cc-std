// Copyright 2022 Google LLC
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
///////////////////////////////////////////////////////////////////////////////
#ifndef TINK_EXAMPLES_WALKTHROUGH_OBTAIN_AND_USE_A_PRIMITIVE_H_
#define TINK_EXAMPLES_WALKTHROUGH_OBTAIN_AND_USE_A_PRIMITIVE_H_

#include <memory>
#include <string>

#include "tink/absl/strings/string_view.h"
#include "tink/keyset_handle.h"
#include "tink/util/statusor.h"

namespace tink_walkthrough {

// AEAD encrypts `plaintext` with `associated_data` and the primary key in
// `keyset_handle`.
crypto::tink::util::StatusOr<std::string> AeadEncrypt(
    const crypto::tink::KeysetHandle& keyset_handle,
    absl::string_view palintext, absl::string_view associated_data);

// AEAD decrypts `ciphertext` with `associated_data` and the correct key in
// `keyset_handle`.
crypto::tink::util::StatusOr<std::string> AeadDecrypt(
    const crypto::tink::KeysetHandle& keyset_handle,
    absl::string_view ciphertext, absl::string_view associated_data);

}  // namespace tink_walkthrough

#endif  // TINK_EXAMPLES_WALKTHROUGH_OBTAIN_AND_USE_A_PRIMITIVE_H_
