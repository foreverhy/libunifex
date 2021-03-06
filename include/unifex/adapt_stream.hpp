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

#include <unifex/stream_concepts.hpp>

#include <functional>
#include <type_traits>

namespace unifex {

template <
    typename Stream,
    typename NextAdaptFunc,
    typename CleanupAdaptFunc>
struct adapted_stream {
  Stream innerStream_;
  NextAdaptFunc nextAdapter_;
  CleanupAdaptFunc cleanupAdapter_;

  friend auto tag_invoke(tag_t<next>, adapted_stream& s)
    -> std::invoke_result_t<NextAdaptFunc&, next_sender_t<Stream>> {
    return std::invoke(s.nextAdapter_, next(s.innerStream_));
  }

  friend auto tag_invoke(tag_t<cleanup>, adapted_stream& s)
    -> std::invoke_result_t<CleanupAdaptFunc&, cleanup_sender_t<Stream>> {
    return std::invoke(s.cleanupAdapter_, cleanup(s.innerStream_));
  }
};

template <typename Stream, typename AdaptFunc>
struct both_adapted_stream {
  Stream innerStream_;
  AdaptFunc adapter_;

  friend auto tag_invoke(tag_t<next>, both_adapted_stream& s)
    -> std::invoke_result_t<AdaptFunc&, next_sender_t<Stream>> {
    return std::invoke(s.adapter_, next(s.innerStream_));
  }

  friend auto tag_invoke(tag_t<cleanup>, both_adapted_stream& s)
    -> std::invoke_result_t<AdaptFunc&, cleanup_sender_t<Stream>> {
    return std::invoke(s.adapter_, cleanup(s.innerStream_));
  }
};

template <typename Stream, typename AdapterFunc>
auto adapt_stream(Stream&& stream, AdapterFunc&& adapt) {
  return both_adapted_stream<
      std::remove_cvref_t<Stream>,
      std::remove_cvref_t<AdapterFunc>>{(Stream &&) stream,
                                        (AdapterFunc &&) adapt};
}

template <
    typename Stream,
    typename NextAdapterFunc,
    typename CleanupAdapterFunc>
auto adapt_stream(
    Stream&& stream,
    NextAdapterFunc&& adaptNext,
    CleanupAdapterFunc&& adaptCleanup) {
  return adapted_stream<
      std::remove_cvref_t<Stream>,
      std::remove_cvref_t<NextAdapterFunc>,
      std::remove_cvref_t<CleanupAdapterFunc>>{(Stream &&) stream,
                                               (NextAdapterFunc &&) adaptNext,
                                               (CleanupAdapterFunc &&)
                                                   adaptCleanup};
}

} // namespace unifex
