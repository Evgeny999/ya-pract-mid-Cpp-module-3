#pragma once

#include <print>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    using BookIterator = BookContainer::iterator;
    using BookConstIterator = BookContainer::const_iterator;
    using AuthorContainer = std::unordered_set<std::string>;

    BookDatabase() = default;
    BookDatabase(std::initializer_list<Book> book_list) {
        for (const auto &book : book_list) {
            books_.push_back(book);
            InsertAuthor(book.author);
        }
    }

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    // Standard container interface methods

    size_t size() const { return books_.size(); }

    BookContainer GetBooks() const { return books_; }
    AuthorContainer GetAuthors() const { return authors_; }
    void PushBack(const Book &b) {
        books_.push_back(b);
        InsertAuthor(b.author);
    }

    void EmplaceBack(std::string_view title, std::string_view author, int year, Genre genre, double rating,
                     int read_count) {
        books_.emplace_back(title, author, year, genre, rating, read_count);
        InsertAuthor(author);
    }

    BookIterator begin() { return books_.begin(); }
    BookIterator end() { return books_.end(); }

    BookConstIterator cbegin() const { return books_.cbegin(); }
    BookConstIterator cend() const { return books_.cend(); }

private:
    // Помещаем имя автора в authors_. string_view в соответствующем элементе вектора books_ будет "указывать" на него
    void InsertAuthor(std::string_view author) {
        authors_.insert(std::string{author});
        books_.back().author = *authors_.find(std::string{books_.back().author});
    }

    BookContainer books_;
    AuthorContainer authors_;
};

}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>> &db, FormatContext &fc) const {

        db.size();

        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}  // namespace std
