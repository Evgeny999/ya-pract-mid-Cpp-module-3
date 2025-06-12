#pragma once

#include <algorithm>
#include <flat_map>
#include <format>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

// Ваш код для constexpr преобразования строк в enum::Genre и наоборот здесь

constexpr Genre GenreFromString(std::string_view s) {
    if (s == "Fiction") {
        return Genre::Fiction;
    }
    if (s == "Mystery") {
        return Genre::Mystery;
    }
    if (s == "NonFiction") {
        return Genre::NonFiction;
    }
    if (s == "SciFi") {
        return Genre::SciFi;
    }
    if (s == "Biography") {
        return Genre::Biography;
    }
    return Genre::Unknown;
}

struct Book {

    constexpr Book(std::string_view title, std::string_view author, int year, std::string_view genre, double rating,
                   int read_count)
        : title(title), author(author), year(year), genre(GenreFromString(genre)), rating(rating),
          read_count(read_count) {}

    constexpr Book(std::string_view title, std::string_view author, int year, Genre genre, double rating,
                   int read_count)
        : title(title), author(author), year(year), genre(genre), rating(rating), read_count(read_count) {}

    // string_view для экономии памяти, чтобы ссылаться на оригинальную строку, хранящуюся в другом контейнере
    std::string title;
    std::string_view author;

    int year;
    Genre genre;
    double rating;
    int read_count;
};

}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::Genre, char> {
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext &fc) const {
        std::string genre_str;

        // clang-format off
        using bookdb::Genre;
        switch (g) {
            case Genre::Fiction:    genre_str = "Fiction"; break;
            case Genre::Mystery:    genre_str = "Mystery"; break;
            case Genre::NonFiction: genre_str = "NonFiction"; break;
            case Genre::SciFi:      genre_str = "SciFi"; break;
            case Genre::Biography:  genre_str = "Biography"; break;
            case Genre::Unknown:    genre_str = "Unknown"; break;
            default:
                throw logic_error{"Unsupported bookdb::Genre"};
            }
        // clang-format on
        return format_to(fc.out(), "{}", genre_str);
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

template <>
struct formatter<bookdb::Book, char> {
    template <typename FormatContext>
    auto format(const bookdb::Book &b, FormatContext &fc) const {

        return format_to(fc.out(), "{} {} {} {} {} {}", b.author, b.title, b.year, b.genre, b.rating, b.read_count);
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

template <>
struct formatter<std::flat_map<std::string, int>, char> {
    template <typename FormatContext>
    auto format(const std::flat_map<std::string, int> &m, FormatContext &fc) const {
        std::stringstream result_string;

        std::for_each(m.begin(), m.end(), [&result_string](const auto &p) {
            std::string temp;
            temp.insert(temp.size(), p.second, '*');
            result_string << std::format("{:<30}:   {}\n", p.first, temp);
        });

        return format_to(fc.out(), "{}", result_string.str());
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

template <>
struct formatter<std::flat_map<bookdb::Genre, double>, char> {
    template <typename FormatContext>
    auto format(const std::flat_map<bookdb::Genre, double> &m, FormatContext &fc) const {
        std::stringstream result_string;

        std::for_each(m.begin(), m.end(), [&fc](const auto &p) {
            std::stringstream temp;
            std::string temp_genre;
            format_to(std::back_inserter(temp_genre), "{}", p.first);
            temp << std::format("{:<30}:   {}\n", temp_genre, p.second);
            format_to(fc.out(), "{}", temp.str());
        });

        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

}  // namespace std

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
constexpr bool any_of(UnaryPred... p) {
    // вернём false, если число преданных передикатов равно нулю
    return [p...](const bookdb::Book &book) { return (false || ... || (p(book))); };
}
