
#include "tink/crypto_format.h"

#include "gtest/gtest.h"
#include "proto/tink.pb.h"
#include <cifuzz/cifuzz.h>
#include <fuzzer/FuzzedDataProvider.h>

using google::crypto::tink::KeysetInfo;
using google::crypto::tink::OutputPrefixType;

namespace crypto {
namespace tink {
namespace {

FUZZ_TEST_SETUP(){}

FUZZ_TEST(const uint8_t *data, size_t size) {

  if (size < 5) {
    return;
  }

  FuzzedDataProvider data_provider(data, size);
  
  // Create a KeyInfo object
  KeysetInfo::KeyInfo key_info;
  key_info.set_output_prefix_type(static_cast<OutputPrefixType>(data_provider.ConsumeIntegral<uint32_t>() % 4));
  key_info.set_key_id(data_provider.ConsumeIntegral<uint32_t>());

  auto prefix_result = CryptoFormat::GetOutputPrefix(key_info);
  
  // If the prefix_result status is not OK and the output_prefix_type is one of the valid values, that would be an error situation.
  // However, in a fuzz test we don't explicitly handle this. We just let the program crash.
  assert(prefix_result.ok() || (key_info.output_prefix_type() != OutputPrefixType::TINK && key_info.output_prefix_type() != OutputPrefixType::CRUNCHY && 
        key_info.output_prefix_type() != OutputPrefixType::LEGACY && key_info.output_prefix_type() != OutputPrefixType::RAW));
}

}
}
}