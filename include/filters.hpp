#pragma once

#include <functional>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {

auto YearBetween(int start_year, int end_year) {
    return [start_year, end_year](const Book &book) { return book.year >= start_year && book.year <= end_year; };
}

auto RatingAbove(double threshold) {
    return [threshold](const Book &book) { return book.rating > threshold; };
}

auto GenreIs(Genre genre) {
    return [genre](const Book &book) { return book.genre == genre; };
}

template <BookPredicate... Pred>
auto all_of(Pred &&...preds) {
    return [... p = std::forward<Pred>(preds)](const Book &book) { return (p(book) && ...); };
}

template <BookPredicate... Pred>
auto any_of(Pred &&...preds) {
    return [... p = std::forward<Pred>(preds)](const Book &book) { return (p(book) || ...); };
}

template <BookIterator It, BookPredicate... Pred>
std::vector<std::reference_wrapper<const Book>> filterBooks(It beg, It end, Pred &&...preds) {
    if (beg == end) {
        return {};
    }
    if (sizeof...(preds) == 0) {
        std::vector<std::reference_wrapper<const Book>> result;
        for (auto it = beg; it != end; ++it) {
            result.push_back(std::cref(*it));
        }
        return result;
    }
    std::vector<std::reference_wrapper<const Book>> result;
    auto combined_pred = all_of(std::forward<Pred>(preds)...);
    for (auto it = beg; it != end; ++it) {
        if (combined_pred(*it)) {
            result.push_back(std::cref(*it));
        }
    }
    return result;
}
}  // namespace bookdb