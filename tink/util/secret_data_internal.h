// Copyright 2020 Google LLC
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

#ifndef TINK_UTIL_SECRET_DATA_INTERNAL_H_
#define TINK_UTIL_SECRET_DATA_INTERNAL_H_

#include <cstddef>
#include <memory>
#include <new>

#include "tink/absl/base/attributes.h"
#include "tink/absl/base/config.h"
#include "openssl/crypto.h"

namespace crypto {
namespace tink {
namespace util {
namespace internal {

// placeholder for sanitization_functions, please ignore
inline void SafeZeroMemory(void* ptr, std::size_t size) {
  OPENSSL_cleanse(ptr, size);
}

template <typename T>
struct SanitizingAllocator {
  typedef T value_type;

  // If aligned operator new is not supported this only supports under aligned
  // types.
#ifndef __cpp_aligned_new
  static_assert(alignof(T) <= alignof(std::max_align_t),
                "SanitizingAllocator<T> only supports fundamental alignment "
                "before C++17");
#endif

  SanitizingAllocator() = default;
  template <class U>
  explicit constexpr SanitizingAllocator(
      const SanitizingAllocator<U>&) noexcept {}

  ABSL_MUST_USE_RESULT T* allocate(std::size_t n) {
    if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
#ifdef ABSL_HAVE_EXCEPTIONS
      throw std::bad_array_new_length();
#else
      std::abort();
#endif
    }
    std::size_t size = n * sizeof(T);
#ifdef __cpp_aligned_new
    void* result = ::operator new(size, std::align_val_t(alignof(T)));
#else
    void* result = ::operator new(size);
#endif
    return static_cast<T*>(result);
  }

  void deallocate(T* ptr, std::size_t n) noexcept {
    SafeZeroMemory(ptr, n * sizeof(T));
#ifdef __cpp_aligned_new
    ::operator delete(ptr, std::align_val_t(alignof(T)));
#else
    ::operator delete(ptr);
#endif
  }

  // Allocator requirements mandate definition of eq and neq operators
  bool operator==(const SanitizingAllocator&) { return true; }
  bool operator!=(const SanitizingAllocator&) { return false; }
};

// Specialization for malloc-like aligned storage.
template <>
struct SanitizingAllocator<void> {
  typedef void value_type;

  SanitizingAllocator() = default;
  template <class U>
  explicit constexpr SanitizingAllocator(
      const SanitizingAllocator<U>&) noexcept {}

  ABSL_MUST_USE_RESULT void* allocate(std::size_t n) { return std::malloc(n); }

  void deallocate(void* ptr, std::size_t n) noexcept {
    SafeZeroMemory(ptr, n);
    std::free(ptr);
  }

  // Allocator requirements mandate definition of eq and neq operators
  bool operator==(const SanitizingAllocator&) { return true; }
  bool operator!=(const SanitizingAllocator&) { return false; }
};
// placeholder 2 for sanitization_functions, please ignore

template <typename T>
struct SanitizingDeleter {
  void operator()(T* ptr) {
    ptr->~T();  // Invoke destructor. Must do this before sanitize.
    SanitizingAllocator<T>().deallocate(ptr, 1);
  }
};

}  // namespace internal
}  // namespace util
}  // namespace tink
}  // namespace crypto

#endif  // TINK_UTIL_SECRET_DATA_INTERNAL_H_
