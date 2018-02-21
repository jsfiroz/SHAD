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


#ifndef INCLUDE_SHAD_RUNTIME_TBB_SYSTEM_UTILITY_H_
#define INCLUDE_SHAD_RUNTIME_TBB_SYSTEM_UTILITY_H_

#include <cstddef>
#include <cstdint>
#include <sstream>
#include <system_error>

#include "shad/runtime/locality.h"

namespace shad {
namespace rt {

namespace impl {

inline void checkLocality(const Locality & loc) {
  Locality L(0);

  if (loc != L) {
    std::stringstream ss;
    ss << "The system does not include " << loc;
    throw std::system_error(0xdeadc0de, std::generic_category(), ss.str());
  }
}

}  // namespace impl

}  // namespace rt
}  // namespace shad


#endif  // INCLUDE_SHAD_RUNTIME_TBB_SYSTEM_UTILITY_H_
