#pragma once

#include <concepts>
#include <iterator>
#include <string_view>

#include "book.hpp"

namespace bookdb {
// books_.emplace_back(title, author, year, genre, rating, read_count);
template <typename T>
concept BookContainerLike = (requires(T &t) {
    { t.begin() } -> std::same_as<typename T::iterator>;
    { t.end() } -> std::same_as<typename T::iterator>;
    { t.cbegin() } -> std::same_as<typename T::const_iterator>;
    { t.cend() } -> std::same_as<typename T::const_iterator>;
    { t.clear() } -> std::same_as<void>;
    { t.size() } -> std::same_as<std::size_t>;
    { t.push_back(std::declval<typename T::value_type>()) } -> std::same_as<void>;
    // Для emplace back нужно здесь передавать аргументы (title, author, year, genre, rating, read_count)?
    // Получается, что сейчас проверяется только наличие этого метода. А можно ли проверить, что именно только этими
    // аргументами он компилируется?
    { t.emplace_back() } -> std::same_as<typename T::value_type &>;
} && std::ranges::range<T>);

template <typename T>
concept BookIterator = std::random_access_iterator<T>;

template <typename S, typename I>
concept BookSentinel = std::sentinel_for<S, I>;

template <typename P>
concept BookPredicate = std::predicate<P, const bookdb::Book &>;
// А где BookComparator нужно было использовать?
template <typename C>
concept BookComparator = true;

}  // namespace bookdb