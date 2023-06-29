///////////////////////////////////////////////////////////////////////////////
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

#include "tink/subtle/aes_cmac_wolfssl.h"

#include <cstdint>
#include <memory>
#include <string>
#include <utility>

#include "tink/absl/memory/memory.h"
#include "tink/absl/status/status.h"
#include "wolfssl/wolfcrypt/cmac.h"
#include "wolfssl/wolfcrypt/error-crypt.h"
#include "tink/internal/aes_util.h"
#include "tink/internal/util.h"
#include "tink/subtle/subtle_util.h"
#include "tink/util/errors.h"
#include "tink/util/status.h"

namespace crypto {
namespace tink {
namespace subtle {

// CMAC key sizes in bytes.
// The small key size is used only to check RFC 4493's test vectors due to
// the attack described in
// https://www.math.uwaterloo.ca/~ajmeneze/publications/tightness.pdf. We
// check this restriction in AesCmacManager.
static constexpr size_t kSmallKeySize = 16;
static constexpr size_t kBigKeySize = 32;
static constexpr size_t kMaxTagSize = 16;

// static
util::StatusOr<std::unique_ptr<Mac>> AesCmacWolfssl::New(
    util::SecretData key, uint32_t tag_size) {
  if (key.size() != kSmallKeySize && key.size() != kBigKeySize) {
    return util::Status(util::error::INVALID_ARGUMENT,
                        absl::StrCat("Invalid key size: expected ",
                                     kSmallKeySize, " or ", kBigKeySize,
                                     ", found ", key.size()));
  }
  if (tag_size > kMaxTagSize) {
    return util::Status(
        util::error::INVALID_ARGUMENT,
        absl::StrCat("Invalid tag size: expected lower than ", kMaxTagSize,
                     ", found ", tag_size));
  }
  return absl::WrapUnique(new AesCmacWolfssl(std::move(key), tag_size));
}

util::StatusOr<std::string> AesCmacWolfssl::ComputeMac(
    absl::string_view data) const {
  WOLFSSL_CMAC_CTX cmac;
  int ret = wc_InitCmac(&cmac, nullptr, 0);
  if (ret != 0) {
    return util::Status(util::error::INTERNAL, "Failed to initialize CMAC");
  }

  ret = wc_CmacSetKey(&cmac, WC_CIPHER_AES, key_.data(), nullptr, key_.size());
  if (ret != 0) {
    return util::Status(util::error::INTERNAL, "Failed to set CMAC key");
  }

  std::string result(tag_size_, '\0');
  unsigned long outLen = tag_size_;
  ret = wc_CmacUpdate(&cmac, reinterpret_cast<const uint8_t*>(data.data()),
                      data.size());
  if (ret != 0) {
    return util::Status(util::error::INTERNAL, "Failed to update CMAC");
  }

  ret = wc_CmacFinal(&cmac, reinterpret_cast<uint8_t*>(&result[0]), &outLen);
  if (ret != 0) {
    return util::Status(util::error::INTERNAL, "Failed to finalize CMAC");
  }

  result.resize(outLen);
  return result;
}

util::Status AesCmacWolfssl::VerifyMac(absl::string_view mac,
                                       absl::string_view data) const {
  if (mac.size() != tag_size_) {
    return util::Status(util::error::INVALID_ARGUMENT,
                        absl::StrCat("Incorrect tag size: expected ", tag_size_,
                                     ", found ", mac.size()));
  }

  util::StatusOr<std::string> computed_mac = ComputeMac(data);
  if (!computed_mac.ok()) {
    return computed_mac.status();
  }

  if (computed_mac.ValueOrDie() != std::string(mac)) {
    return util::Status(util::error::INVALID_ARGUMENT, "CMAC verification failed");
  }

  return util::OkStatus();
}

}  // namespace subtle
}  // namespace tink
}  // namespace crypto
