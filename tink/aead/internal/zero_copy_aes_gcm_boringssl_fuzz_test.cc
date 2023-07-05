
#include "tink/aead/internal/zero_copy_aes_gcm_boringssl.h"

#include <algorithm>
#include <cstring>
#include <iterator>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "tink/absl/status/status.h"
#include "tink/absl/strings/escaping.h"
#include "tink/absl/strings/str_cat.h"
#include "tink/absl/types/span.h"
#include "tink/aead/internal/zero_copy_aead.h"
#include "tink/subtle/subtle_util.h"
#include "tink/util/statusor.h"

#include <cifuzz/cifuzz.h>
#include <fuzzer/FuzzedDataProvider.h>

namespace crypto {
namespace tink {
namespace internal {
namespace {


FUZZ_TEST_SETUP(){}

FUZZ_TEST(const uint8_t *data, size_t size) {
  if (size < 1) {
    return;
  }

  FuzzedDataProvider data_provider(data, size);

  // Randomly generate the key.
  std::string random_key = data_provider.ConsumeRandomLengthString(256);

  // Initialize the cipher.
  util::SecretData key = util::SecretDataFromStringView(random_key);
  auto cipher_status = ZeroCopyAesGcmBoringSsl::New(key);

  // If cipher creation is not successful, there is no point in continuing the test.
  if (!cipher_status.ok()) {
    return;
  }

  auto cipher = std::move(*cipher_status);

  // Generate a random message size between 0 and 256.
  size_t message_size = data_provider.ConsumeIntegral<size_t>();

  std::string message = data_provider.ConsumeRandomLengthString(message_size);
  std::string associated_data = data_provider.ConsumeRemainingBytesAsString();

  std::string ciphertext;
  subtle::ResizeStringUninitialized(&ciphertext, cipher->MaxEncryptionSize(message.size()));
  auto ciphertext_size = cipher->Encrypt(message, associated_data, absl::MakeSpan(ciphertext));

  // If encryption is not successful, stop the test.
  if (!ciphertext_size.ok()) {
    return;
  }

  std::string plaintext;
  subtle::ResizeStringUninitialized(&plaintext, cipher->MaxDecryptionSize(ciphertext.size()));
  auto plaintext_size = cipher->Decrypt(ciphertext, associated_data, absl::MakeSpan(plaintext));

  // If decryption is not successful, stop the test.
  if (!plaintext_size.ok()) {
    return;
  }

  // Check that the decrypted text is the same as the original message. If it's not, the program will crash,
  // which indicates a failure in the fuzzing test.
  assert(plaintext == message);
}

}  // namespace
}  // namespace internal
}  // namespace tink
}  // namespace crypto