#pragma once

#include "book.hpp"
#include <string>
#include <string_view>

namespace bookdb {

struct TransparentStringLess {
    using is_transparent = void;
    // Гетерогенный компаратор по названию
    bool operator()(const std::string &lhs, const Book &rhs) { return lhs < rhs.title; }
    bool operator()(const Book &lhs, const std::string &rhs) { return lhs.title < rhs; }
    bool operator()(const std::string &lhs, const std::string &rhs) { return lhs < rhs; }
    bool operator()(const Book &lhs, const Book &rhs) { return lhs.title < rhs.title; }
};

struct TransparentStringEqual {
    using is_transparent = void;
    // Гетерогенный компаратор по названию
    bool operator()(const std::string &lhs, const Book &rhs) { return lhs == rhs.title; }
    bool operator()(const Book &lhs, const std::string &rhs) { return lhs.title == rhs; }
    bool operator()(const std::string &lhs, const std::string &rhs) { return lhs == rhs; }
    bool operator()(const Book &lhs, const Book &rhs) { return lhs.title == rhs.title; }
};

struct TransparentStringHash {
    using is_transparent = void;  // поддержка гетерогенного поиска

    // получение хеша для элемента класса Book
    std::size_t operator()(const Book &key) const { return std::hash<std::string>{}(key.title); }

    // гетерогенное получение хеша
    std::size_t operator()(const std::string &title) const { return std::hash<std::string>{}(title); }
};

}  // namespace bookdb
