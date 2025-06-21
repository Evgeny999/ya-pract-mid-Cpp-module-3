#pragma once

#include <algorithm>
#include <flat_map>
#include <iterator>
#include <map>
#include <numeric>
#include <random>
#include <stdexcept>
#include <string>
#include <string_view>

#include "book.hpp"
#include "book_database.hpp"
#include "comparators.hpp"

#include <print>
#include <vector>

namespace bookdb {

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont, Comparator comp = {}) {
    std::flat_map<std::string, int> result;
    std::for_each(cont.cbegin(), cont.cend(), [&result](const bookdb::Book &book) {
        auto [iter, _] = result.emplace(std::string{book.author}, 0);
        iter->second++;
    });
    return result;
}

template <BookContainerLike T>
auto calculateGenreRatings(const BookDatabase<T> &cont) {
    struct BookRatingCount {
        int counter = 0;
        double rating = 0;
    };

    std::map<bookdb::Genre, BookRatingCount> books_rating_count;
    std::for_each(cont.cbegin(), cont.cend(), [&books_rating_count](const bookdb::Book &book) {
        if (!books_rating_count.contains(book.genre)) {
            books_rating_count[book.genre] = {1, book.rating};
        } else {
            ++books_rating_count[book.genre].counter;
            books_rating_count[book.genre].rating += book.rating;
        }
    });

    std::flat_map<bookdb::Genre, double> result;

    std::for_each(books_rating_count.cbegin(), books_rating_count.cend(), [&result](const auto &pair) {
        result.insert_or_assign(pair.first, pair.second.rating / pair.second.counter);
    });

    return result;
}

template <BookContainerLike T>
auto calculateAverageRating(const BookDatabase<T> &cont) {
    auto sum =
        std::accumulate(cont.cbegin(), cont.cend(), 0.0, [](auto part_sum, auto rhs) { return part_sum + rhs.rating; });
    return sum / cont.size();
}

template <BookContainerLike T>
auto sampleRandomBooks(const BookDatabase<T> &cont, size_t num_books) {

    if (num_books > cont.size()) {
        num_books = cont.size();
    }

    std::vector<size_t> indices(cont.size());
    for (size_t i = 0; i < cont.size(); ++i) {
        indices[i] = i;
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);

    std::vector<std::reference_wrapper<const bookdb::Book>> result;
    for (size_t i = 0; i < num_books; ++i) {
        result.push_back(std::ref(*(cont.begin() + indices[i])));
    }
    return result;
}

template <BookContainerLike T, typename Comparator = comp::GreaterByRating>
auto getTopNBy(BookDatabase<T> &cont, int num, Comparator comp = {}) {
    std::partial_sort(cont.begin(), cont.begin() + num, cont.end(), comp);
    std::vector<std::reference_wrapper<const bookdb::Book>> result(cont.begin(), cont.begin() + num);
    return result;
}

}  // namespace bookdb
