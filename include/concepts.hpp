#pragma once

#include <concepts>
#include <iterator>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookIterator =
    std::random_access_iterator<T> && std::same_as<typename std::iterator_traits<T>::value_type, Book>;

template <typename T>
concept BookContainerLike = requires(T t) {
    requires BookIterator<typename T::iterator>;
    typename T::const_iterator;
    typename T::size_type;
    { t.begin() } -> std::same_as<typename T::iterator>;
    { t.end() } -> std::same_as<typename T::iterator>;
    { t.cbegin() } -> std::same_as<typename T::const_iterator>;
    { t.cend() } -> std::same_as<typename T::const_iterator>;
    { t.size() } -> std::same_as<typename T::size_type>;
    { t.push_back(std::declval<Book>()) } -> std::same_as<void>;
    { t.clear() } -> std::same_as<void>;
    { t.emplace_back(std::declval<Book>()) } -> std::same_as<Book &>;
    { t.reserve(typename T::size_type{}) } -> std::same_as<void>;
};

template <typename S, typename I>
concept BookSentinel = std::sentinel_for<I, S> && BookIterator<I>;

template <typename P>
concept BookPredicate = requires(const Book &b, P p) {
    { p(b) } -> std::convertible_to<bool>;
};

template <typename C>
concept BookComparator = requires(const Book &a, const Book &b, C c) {
    { c(a, b) } -> std::convertible_to<bool>;
};

}  // namespace bookdb