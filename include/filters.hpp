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

constexpr auto YearBetween(int start, int end) {
    return [start, end](const bookdb::Book &book) { return book.year >= start && book.year <= end; };
}

constexpr auto RatingAbove(double rating) {
    return [rating](const bookdb::Book &book) { return book.rating >= rating; };
}

constexpr auto GenreIs(bookdb::Genre genre) {
    return [genre](const bookdb::Book &book) { return book.genre == genre; };
}

template <class... UnaryPred>
constexpr auto all_of(UnaryPred... p) {
    // вернём true, если число преданных передикатов равно нулю
    return [p...](const bookdb::Book &book) { return (true && ... && (p(book))); };
}

template <class... UnaryPred>
constexpr auto any_of(UnaryPred... p) {
    // вернём false, если число преданных передикатов равно нулю
    return [p...](const bookdb::Book &book) { return (false || ... || (p(book))); };
}