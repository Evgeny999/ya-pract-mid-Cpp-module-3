#pragma once

#include <format>
#include <stdexcept>
#include <string>
#include <string_view>

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
        std::string genre_str;

        // clang-format off
        using bookdb::Genre;
        switch (b.genre) {
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
        return format_to(fc.out(), "{} {} {} {} {} {}", b.author, b.title, b.year, genre_str, b.rating, b.read_count);
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
    return [genre](const bookdb::Book &book) { return book.genre >= genre; };
}

template <class InputIt, class... UnaryPred>
constexpr bool all_of(InputIt first, InputIt last, UnaryPred... p) {
    // вернём true, если число преданных передикатов равно нулю
    return (true && ... && (std::find_if_not(first, last, p) == last));
}

template <class InputIt, class... UnaryPred>
constexpr bool any_of(InputIt first, InputIt last, UnaryPred... p) {
    // вернём false, если число преданных передикатов равно нулю
    return (false || ... || (std::find_if_not(first, last, p) != last));
}
