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
        if (!result.contains(std::string{book.author})) {
            result[std::string{book.author}] = 1;
        } else {
            ++result[std::string{book.author}];
        }
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
            auto key = book.genre;
            books_rating_count[key] = {1, book.rating};
        } else {
            auto key = book.genre;
            ++books_rating_count[key].counter;
            books_rating_count[key].rating += book.rating;
        }
    });

    std::flat_map<bookdb::Genre, double> result;

    std::for_each(books_rating_count.cbegin(), books_rating_count.cend(), [&result](const auto &pair) {
        auto key = pair.first;
        result[key] = pair.second.rating / pair.second.counter;
    });

    // for (const auto &pair : books_rating_count) {
    // clang-format off
        /*
        Если делать напрямую result[pair.first] =  ..., то:
error: cannot bind rvalue reference of type 'const bookdb::Genre&&' to lvalue of type 'const std::_Flat_map_impl<bookdb::Genre, double, std::less<bookdb::Genre>, std::vector<bookdb::Genre, std::allocator<bookdb::Genre> >, std::vector<double>, false>::key_type' {aka 'const bookdb::Genre'}
 1145 |       { return operator[]<const key_type>(__x); }
      |                ~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~
      Не очень понял - почему. Кстати с обычным map result[pair.first] бы компилировалось
        */
    // clang-format on
    // result[pair.first] = pair.second.rating / pair.second.counter;
    // }
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
