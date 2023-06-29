// Copyright 2013 Google Inc. All Rights Reserved.
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

#ifndef TINK_UTIL_STATUSOR_H_
#define TINK_UTIL_STATUSOR_H_

#include "tink/absl/status/statusor.h"
#include "tink/util/status.h"

#define TINK_USE_ABSL_STATUSOR

namespace crypto {
namespace tink {
namespace util {

template <typename T>
using StatusOr = absl::StatusOr<T>;

}  // namespace util
}  // namespace tink
}  // namespace crypto

#endif  // TINK_UTIL_STATUSOR_H_
