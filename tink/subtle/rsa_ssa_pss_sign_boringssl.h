// Copyright 2018 Google Inc.
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

#ifndef TINK_SUBTLE_RSA_SSA_PSS_SIGN_BORINGSSL_H_
#define TINK_SUBTLE_RSA_SSA_PSS_SIGN_BORINGSSL_H_

#include <memory>
#include <string>
#include <utility>

#include "tink/absl/strings/string_view.h"
#include "openssl/ec.h"
#include "openssl/rsa.h"
#include "tink/internal/fips_utils.h"
#include "tink/internal/rsa_util.h"
#include "tink/internal/ssl_unique_ptr.h"
#include "tink/public_key_sign.h"
#include "tink/subtle/common_enums.h"
#include "tink/util/statusor.h"

namespace crypto {
namespace tink {
namespace subtle {

// The RSA SSA (Signature Schemes with Appendix) using PSS (Probabilistic
// Signature Scheme) encoding is defined at
// https://tools.ietf.org/html/rfc8017#section-8.1).
class RsaSsaPssSignBoringSsl : public PublicKeySign {
 public:
  static crypto::tink::util::StatusOr<std::unique_ptr<PublicKeySign>> New(
      const crypto::tink::internal::RsaPrivateKey& private_key,
      const crypto::tink::internal::RsaSsaPssParams& params);

  ~RsaSsaPssSignBoringSsl() override = default;

  crypto::tink::util::StatusOr<std::string> Sign(
      absl::string_view data) const override;

  static constexpr crypto::tink::internal::FipsCompatibility kFipsStatus =
      crypto::tink::internal::FipsCompatibility::kRequiresBoringCrypto;

 private:
  RsaSsaPssSignBoringSsl(crypto::tink::internal::SslUniquePtr<RSA> private_key,
                         const EVP_MD* sig_hash, const EVP_MD* mgf1_hash,
                         int32_t salt_length)
      : private_key_(std::move(private_key)),
        sig_hash_(sig_hash),
        mgf1_hash_(mgf1_hash),
        salt_length_(salt_length) {}

  const crypto::tink::internal::SslUniquePtr<RSA> private_key_;
  // Pointers to singletons owned by OpenSSL/BoringSSL.
  const EVP_MD* sig_hash_;
  const EVP_MD* mgf1_hash_;
  const int32_t salt_length_;
};

}  // namespace subtle
}  // namespace tink
}  // namespace crypto

#endif  // TINK_SUBTLE_RSA_SSA_PSS_SIGN_BORINGSSL_H_
