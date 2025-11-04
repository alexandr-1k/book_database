#pragma once

#include <algorithm>
#include <initializer_list>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"
#include <unordered_set>

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    using AuthorContainer = std::unordered_set<std::string>;

    using iterator = typename BookContainer::iterator;
    using const_iterator = typename BookContainer::const_iterator;
    using size_type = typename BookContainer::size_type;

    BookDatabase() = default;

    BookDatabase(std::initializer_list<Book> books) {
        books_.reserve(books.size());
        for (const auto &book : books) {
            books_.push_back(book);
            auto &stored = books_.back();
            auto it = InsertAuthor(stored.author);
            stored.author = *it;
        }
    }

    iterator begin() { return books_.begin(); }
    const_iterator begin() const { return books_.cbegin(); }
    const_iterator cbegin() const { return books_.cbegin(); }

    iterator end() { return books_.end(); }
    const_iterator end() const { return books_.cend(); }
    const_iterator cend() const { return books_.cend(); }

    size_type size() { return books_.size(); }
    size_type size() const { return books_.size(); }

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    void PushBack(const Book &book) {
        Book temp = book;

        auto it = InsertAuthor(temp.author);
        temp.author = std::string_view(*it);

        books_.push_back(std::move(temp));
    }

    void PushBack(Book &&book) {
        auto it = InsertAuthor(book.author);
        book.author = std::string_view(*it);

        books_.push_back(std::move(book));
    }

    template <typename... Args>
    Book &EmplaceBack(Args &&...args) {
        Book temp = Book(std::forward<Args>(args)...);

        auto it = InsertAuthor(temp.author);
        temp.author = std::string_view(*it);

        books_.push_back(std::move(temp));
        return books_.back();
    }

    const BookContainer &GetBooks() const { return books_; }
    const AuthorContainer &GetAuthors() const { return authors_; }
    BookContainer &GetBooks() { return books_; }

private:
    BookContainer books_;
    AuthorContainer authors_;

    auto InsertAuthor(std::string_view author) {
        auto [it, ok] = authors_.insert(std::string{author});
        if (!ok) {
            it = authors_.find(std::string{author});
            if (it == authors_.end()) {
                throw std::runtime_error{"Could not find author being inserted"};
            }
        }
        return it;
    }
};

}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>> &db, FormatContext &fc) const {

        format_to(fc.out(), "Book Database (size = {}):\n", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        std::vector<std::string_view> sorted_views(db.GetAuthors().begin(), db.GetAuthors().end());
        std::sort(sorted_views.begin(), sorted_views.end());
        format_to(fc.out(), "Authors:\n");
        for (const auto &author : sorted_views) {
            format_to(fc.out(), "- {}\n", author);
        }

        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
};
}  // namespace std
