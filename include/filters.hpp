#pragma once

#include <algorithm>
#include <functional>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {
template <BookIterator It, BookSentinel<It> Sent, BookPredicate Pred>
auto filterBooks(It first, Sent last, Pred pred) {
    std::vector<std::reference_wrapper<const bookdb::Book>> result;
    std::for_each(first, last, [&result, pred](const auto &book) {
        if (pred(book)) {
            result.push_back(std::ref(book));
        }
    });
    return result;
}
}  // namespace bookdb