#pragma once
#include "book.hpp"
#include "book_database.hpp"
#include "comparators.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"
#include <algorithm>
#include <flat_map>
#include <numeric>
#include <random>

namespace bookdb {

template <BookContainerLike T, BookComparator Comparator = TransparentCompareBookByAuthor>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont, Comparator comp = {}) {
    std::flat_map<Book, size_t, Comparator> histogram{comp};
    for (const auto &book : cont.GetBooks()) {
        auto [it, inserted] = histogram.try_emplace(book, 0);
        ++it->second;
    }
    return histogram;
}

template <BookContainerLike T>
auto calculateGenreRatings(const BookDatabase<T> &cont) {
    std::flat_map<Genre, std::pair<double, size_t>> genreRatings;
    for (const auto &book : cont.GetBooks()) {
        auto [it, inserted] = genreRatings.try_emplace(book.genre, std::pair<double, size_t>{0.0, 0});
        it->second.first += book.rating;
        it->second.second += 1;
    }

    std::flat_map<Genre, double> averageRatings;
    for (const auto &[genre, ratingData] : genreRatings) {
        averageRatings.try_emplace(genre, static_cast<double>(ratingData.first) / ratingData.second);
    }
    return averageRatings;
}

template <BookContainerLike T>
auto calculateAverageRating(const BookDatabase<T> &cont) {
    return std::accumulate(cont.GetBooks().begin(), cont.GetBooks().end(), 0.0,
                           [](double acc, const Book &book) { return acc + book.rating; }) /
           cont.GetBooks().size();
}

template <BookContainerLike T>
auto sampleRandomBooks(const BookDatabase<T> &cont, size_t sample_size) {
    std::vector<std::reference_wrapper<const Book>> sampled_books;
    sampled_books.reserve(sample_size);

    std::sample(cont.GetBooks().begin(), cont.GetBooks().end(), std::back_inserter(sampled_books), sample_size,
                std::mt19937{std::random_device{}()});

    return sampled_books;
}

template <BookContainerLike T, BookComparator Comparator = comp::BiggerByRating>
auto getTopNBy(BookDatabase<T> &cont, size_t n, Comparator comp = {}) {
    std::vector<std::reference_wrapper<const Book>> top_n_books;
    top_n_books.reserve(std::min(n, cont.GetBooks().size()));
    std::partial_sort(cont.GetBooks().begin(), cont.GetBooks().begin() + n, cont.GetBooks().end(), comp);

    for (size_t i = 0; i < std::min(n, cont.GetBooks().size()); ++i) {
        top_n_books.push_back(std::cref(cont.GetBooks()[i]));
    }
    return top_n_books;
}

template <typename Map>
auto map_to_string(const Map &map) {
    std::string result;
    for (const auto &[k, v] : map) {
        result += std::format("{}: {}\n", k, v);
    }
    return result;
}

}  // namespace bookdb
