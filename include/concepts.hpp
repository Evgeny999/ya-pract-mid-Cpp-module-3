#pragma once

#include <concepts>
#include <iterator>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = (requires(T &t, typename T::value_type value) {
    { t.size() } -> std::same_as<std::size_t>;
    { t.push_back(value) } -> std::same_as<void>;
} && std::ranges::range<T>);

template <typename T>
concept BookIterator = true;

template <typename S, typename I>
concept BookSentinel = std::sentinel_for<S, I>;

template <typename P>
concept BookPredicate = true;

template <typename C>
concept BookComparator = true;

}  // namespace bookdb