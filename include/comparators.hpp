#pragma once

#include "book.hpp"

#include <float.h>
#include <string>
#include <string_view>

namespace bookdb::comp {

struct LessByAuthor {
    constexpr bool operator()(std::string_view &lhs, std::string_view &rhs) const { return lhs < rhs; }
};

struct LessByTitle {
    constexpr bool operator()(std::string &lhs, std::string &rhs) const { return lhs < rhs; }
};

struct LessByYear {
    constexpr bool operator()(int lhs, int rhs) const { return lhs < rhs; }
};

struct LessByGenre {
    constexpr bool operator()(Genre lhs, Genre rhs) const { return lhs < rhs; }
};

struct LessByRating {
    constexpr bool operator()(double lhs, double rhs) const { return lhs < rhs; }
};

struct LessByReadCount {
    constexpr bool operator()(int lhs, int rhs, double epsilon = DBL_EPSILON) const { return lhs < (rhs - epsilon); }
};

}  // namespace bookdb::comp