/*
 * Copyright 2019-present Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <unifex/config.hpp>
#include <unifex/adapt_stream.hpp>
#include <unifex/scheduler_concepts.hpp>

#include <type_traits>

namespace unifex
{
  template <typename Stream, typename Scheduler, typename Duration>
  auto delay(Stream&& stream, Scheduler&& scheduler, Duration&& duration) {
    return adapt_stream(
        (Stream &&) stream,
        [scheduler = (Scheduler &&) scheduler,
         duration = (Duration &&) duration](auto&& sender) {
          return typed_via(
              schedule_after(scheduler, duration),
              static_cast<decltype(sender)>(sender));
        });
  }
}  // namespace unifex
