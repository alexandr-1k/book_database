#pragma once

#include "book.hpp"

namespace bookdb::comp {

struct LessByAuthor {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.author < rhs.author; }
};

struct LessByYear {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.year < rhs.year; }
};

struct LessByRating {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.year < rhs.year; }
};

struct LessByPopularity {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.read_count < rhs.read_count; }
};

struct BiggerByRating {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.rating > rhs.rating; }
};

}  // namespace bookdb::comp