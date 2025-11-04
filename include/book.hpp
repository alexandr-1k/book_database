#pragma once

#include <algorithm>
#include <format>
#include <stdexcept>
#include <string_view>

namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

constexpr char tolower_constexpr(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

constexpr Genre GenreFromString(std::string_view s) {
    std::string val;
    val.resize(s.size());
    std::transform(s.begin(), s.end(), val.begin(), [](char c) { return tolower_constexpr(c); });

    if (val == "fiction") {
        return Genre::Fiction;
    } else if (val == "nonfiction") {
        return Genre::NonFiction;
    } else if (val == "scifi") {
        return Genre::SciFi;
    } else if (val == "biography") {
        return Genre::Biography;
    } else if (val == "mystery") {
        return Genre::Mystery;
    } else {
        return Genre::Unknown;
    }
}

static std::string GenreToString(Genre g) {
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
            throw std::logic_error{"Unsupported bookdb::Genre"};
        }
    // clang-format on
    return genre_str;
}

struct Book {
    std::string title;
    std::string_view author;

    int year = 0;
    Genre genre;
    double rating = 0.0;
    int read_count = 0;

    constexpr Book(std::string genre_str) : genre(GenreFromString(genre_str)) {}
    constexpr Book(Genre g) : genre(g) {};
    Book(std::string title_, std::string_view author_, int year_, Genre genre_, double rating_, int read_count_)
        : title(std::move(title_)), author(author_), year(year_), genre(genre_), rating(rating_),
          read_count(read_count_) {}
};
}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::Genre, char> {
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext &fc) const {
        return format_to(fc.out(), "{}", GenreToString(g));
    }

    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
};

template <>
struct formatter<bookdb::Book, char> {
    template <typename FormatContext>
    auto format(const bookdb::Book book, FormatContext &fc) const {
        std::string book_repr;
        auto genre_str = bookdb::GenreToString(book.genre);
        return format_to(fc.out(), "{} by {} ({}, {}) Rating: {:.1f} Read Count: {}", book.title, book.author,
                         genre_str, book.year, book.rating, book.read_count);
    }

    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
};

}  // namespace std
