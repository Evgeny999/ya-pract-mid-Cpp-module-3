#pragma once

#include "book.hpp"

#include <float.h>
#include <string>
#include <string_view>

namespace bookdb::comp {

struct LessByAuthor {
    constexpr bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const {
        return lhs.author < rhs.author;
    }
};

struct LessByTitle {
    constexpr bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.title < rhs.title; }
};

struct LessByYear {
    constexpr bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.year < rhs.year; }
};

struct LessByGenre {
    constexpr bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.genre < rhs.genre; }
};

struct LessByRating {
    constexpr bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs, double epsilon = DBL_EPSILON) const {
        return lhs.rating < (rhs.rating - epsilon);
    }
};
// Чтобы получать topN, нужно же использовать greater?
struct GreaterByRating {
    constexpr bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs, double epsilon = DBL_EPSILON) const {
        return lhs.rating > (rhs.rating - epsilon);
    }
};

struct LessByReadCount {
    constexpr bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const {
        return lhs.read_count < rhs.read_count;
    }
};

}  // namespace bookdb::comp