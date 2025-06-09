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

#include <print>
#include <vector>

namespace bookdb {

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont, Comparator comp = {}) {
    std::flat_map<std::string, int> result;
    for (auto book = cont.begin(); book != cont.end(); ++book) {
        if (result.contains(book.author)) {
            result[book.author] = 1;
        } else {
            ++result[book.author];
        }
    }
}

// clang-format off
/*
Тут конечно правильно делать const BookDatabase<T> &cont
Но я не смог побороть:
/workspaces/ya-pract-mid-Cpp-module-3/include/book_database.hpp:51:53: error: could not convert '((const bookdb::BookDatabase<std::vector<bookdb::Book> >*)this)->bookdb::BookDatabase<std::vector<bookdb::Book> >::books_.std::vector<bookdb::Book>::begin()' from '__normal_iterator<const bookdb::Book*,[...]>' to '__normal_iterator<bookdb::Book*,[...]>'
   51 |     BookIterator begin() const { return books_.begin(); }
      |                                         ~~~~~~~~~~~~^~
      |                                                     |
      |                                                     __normal_iterator<const bookdb::Book*,[...]>
Не очень понимаю, что это значит.
*/
// clang-format on

template <BookContainerLike T>
auto calculateGenreRatings(/*const*/ BookDatabase<T> &cont) {
    struct BookRatingCount {
        int counter = 0;
        double rating = 0;
    };

    std::flat_map<bookdb::Genre, BookRatingCount> books_rating_count;
    for (auto book = cont.begin(); book != cont.end(); ++book) {
        if (books_rating_count.contains(book->genre)) {
            books_rating_count[book->genre] = {1, book->rating};
        } else {
            ++books_rating_count[book->genre].counter;
            books_rating_count[book->genre].rating += book->rating;
        }
    }
    std::flat_map<bookdb::Genre, double> result;

    for (const auto &pair : books_rating_count) {
        // clang-format off
        /*
        Если делать напрямую result[pair.first] =  ..., то:
error: cannot bind rvalue reference of type 'const bookdb::Genre&&' to lvalue of type 'const std::_Flat_map_impl<bookdb::Genre, double, std::less<bookdb::Genre>, std::vector<bookdb::Genre, std::allocator<bookdb::Genre> >, std::vector<double>, false>::key_type' {aka 'const bookdb::Genre'}
 1145 |       { return operator[]<const key_type>(__x); }
      |                ~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~
      Не очень понял - почему. Кстати с обычным map result[pair.first] бы компилировалось
        */
        // clang-format on
        auto key = pair.first;
        result[key] = pair.second.rating / pair.second.counter;
    }
    return result;
}

template <BookContainerLike T>
auto calculateAverageRating(/*const*/ BookDatabase<T> &cont) {
    auto sum =
        std::accumulate(cont.begin(), cont.end(), 0.0, [](auto part_sum, auto rhs) { return part_sum + rhs.rating; });
    return sum / cont.size();
}

template <BookContainerLike T>
auto sampleRandomBooks(/*const*/ BookDatabase<T> &cont, size_t num_books) {
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

}  // namespace bookdb
