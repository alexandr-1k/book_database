#pragma once

#include "book.hpp"
#include <string>
#include <string_view>
#include <type_traits>

namespace bookdb {

struct TransparentStringLess {
    using is_transparent = std::true_type;
    bool operator()(const std::string &lhs, const std::string &rhs) const { return lhs < rhs; }
    bool operator()(const std::string &lhs, std::string_view rhs) const { return lhs < rhs; }
    bool operator()(std::string_view lhs, const std::string &rhs) const { return lhs < rhs; }
    bool operator()(std::string_view lhs, std::string_view rhs) const { return lhs < rhs; }
};

struct TransparentStringEqual {
    using is_transparent = std::true_type;
    bool operator()(const std::string &lhs, const std::string &rhs) const { return lhs == rhs; }
    bool operator()(const std::string &lhs, std::string_view rhs) const { return lhs == rhs; }
    bool operator()(std::string_view lhs, const std::string &rhs) const { return lhs == rhs; }
    bool operator()(std::string_view lhs, std::string_view rhs) const { return lhs == rhs; }
};

struct TransparentStringHash {
    using is_transparent = std::true_type;

    auto operator()(const std::string &book) const { return std::hash<std::string>{}(book); }
    auto operator()(std::string_view book) const { return std::hash<std::string_view>{}(book); }
};

struct TransparentCompareBookByAuthor {
    using is_transparent = std::true_type;

    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.author < rhs.author; }
    bool operator()(const bookdb::Book &lhs, std::string_view rhs) const { return lhs.author < rhs; }

    bool operator()(std::string_view lhs, const bookdb::Book &rhs) const { return lhs < rhs.author; }

    bool operator()(std::string_view lhs, std::string_view rhs) const { return lhs < rhs; }
};

}  // namespace bookdb
