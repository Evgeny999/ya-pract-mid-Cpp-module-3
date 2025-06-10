#pragma once

#include <algorithm>
#include <functional>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {
template <class InputIt, class... UnaryPred>
constexpr bool filterBooks(InputIt first, InputIt last, std::function<bool(UnaryPred...)>) {
    // вернём false, если число преданных передикатов равно нулю
}
}  // namespace bookdb