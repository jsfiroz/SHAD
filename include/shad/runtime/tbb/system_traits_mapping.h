//===------------------------------------------------------------*- C++ -*-===//
//
//                                     SHAD
//
//      The Scalable High-performance Algorithms and Data Structure Library
//
//===----------------------------------------------------------------------===//
//
// Copyright 2017 Pacific Northwest National Laboratory
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not
// use this file except in compliance with the License. You may obtain a copy
// of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations
// under the License.
//
//===----------------------------------------------------------------------===//


#ifndef INCLUDE_SHAD_RUNTIME_TBB_SYSTEM_TRAITS_MAPPING_H_
#define INCLUDE_SHAD_RUNTIME_TBB_SYSTEM_TRAITS_MAPPING_H_

#include <cstdint>
#include <limits>
#include <memory>
#include <mutex>
#include <string>

#include "tbb/task_group.h"
#include "tbb/tbb.h"

#include "shad/runtime/mapping_traits.h"

namespace shad {

namespace rt {
namespace impl {

struct tbb_tag {};

template <>
struct HandleTrait<tbb_tag> {
  using HandleTy    = std::shared_ptr<tbb::task_group>;
  using ParameterTy = std::shared_ptr<tbb::task_group> &;
  using ConstParameterTy = const std::shared_ptr<tbb::task_group> &;

  static void Init(ParameterTy H, ConstParameterTy V) {}

  static HandleTy NullValue() {
    return std::shared_ptr<tbb::task_group>(nullptr);
  }

  static bool Equal(ConstParameterTy lhs, ConstParameterTy rhs) {
    return lhs == rhs;
  }

  static std::string toString(ConstParameterTy H) {
    return "";
  }

  static uint64_t toUnsignedInt(ConstParameterTy H) {
    return reinterpret_cast<uint64_t>(H.get());
  }

  static HandleTy CreateNewHandle() {
    return std::shared_ptr<tbb::task_group>(new tbb::task_group());
  }

  static void WaitFor(ParameterTy H) {
    if (H == nullptr) return;
    H->wait();
  }
};

template <>
struct LockTrait<tbb_tag> {
  using LockTy = std::mutex;

  static void lock(LockTy & L) { L.lock(); }
  static void unlock(LockTy & L) { L.unlock(); }
};

template <>
struct RuntimeInternalsTrait<tbb_tag> {
  static void Initialize(int argc, char *argv[]) { }

  static void Finalize() { }

  static size_t Concurrency() {
    return tbb::tbb_thread::hardware_concurrency();
  }
  static void Yield() { tbb::this_tbb_thread::yield(); }

  static uint32_t ThisLocality() { return 0; }
  static uint32_t NullLocality() { return -1; }
  static uint32_t NumLocalities() { return 1; }
};

}  // namespace impl

using TargetSystemTag = impl::tbb_tag;

}  // namespace rt
}  // namespace shad

#endif  // INCLUDE_SHAD_RUNTIME_TBB_SYSTEM_TRAITS_MAPPING_H_
