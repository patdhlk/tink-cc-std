// Copyright 2023 The Abseil Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef ABSL_SYNCHRONIZATION_INTERNAL_PTHREAD_WAITER_H_
#define ABSL_SYNCHRONIZATION_INTERNAL_PTHREAD_WAITER_H_

#ifndef _WIN32
#include <pthread.h>

#include "tink/absl/base/config.h"
#include "tink/absl/synchronization/internal/kernel_timeout.h"
#include "tink/absl/synchronization/internal/waiter_base.h"

namespace absl {
ABSL_NAMESPACE_BEGIN
namespace synchronization_internal {

#define ABSL_INTERNAL_HAVE_PTHREAD_WAITER 1

class PthreadWaiter : public WaiterCrtp<PthreadWaiter> {
 public:
  PthreadWaiter();

  bool Wait(KernelTimeout t);
  void Post();
  void Poke();

  static constexpr char kName[] = "PthreadWaiter";

 private:
  int TimedWait(KernelTimeout t);

  // REQUIRES: mu_ must be held.
  void InternalCondVarPoke();

  pthread_mutex_t mu_;
  pthread_cond_t cv_;
  int waiter_count_;
  int wakeup_count_;  // Unclaimed wakeups.
};

}  // namespace synchronization_internal
ABSL_NAMESPACE_END
}  // namespace absl

#endif  // ndef _WIN32

#endif  // ABSL_SYNCHRONIZATION_INTERNAL_PTHREAD_WAITER_H_
