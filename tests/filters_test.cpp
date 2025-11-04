#include "book_database.hpp"
#include "filters.hpp"
#include <gtest/gtest.h>

using namespace bookdb;

class TestPredicates : public ::testing::Test {
public:
    Book book1{"Title A", "Author A", 2005, Genre::Fiction, 4.5, 100};
    Book book2{"Title B", "Author B", 2015, Genre::Mystery, 4.0, 150};
};

TEST_F(TestPredicates, YearBetween) {
    auto filter = bookdb::YearBetween(2000, 2010);

    EXPECT_TRUE(filter(book1));
    EXPECT_FALSE(filter(book2));
}

TEST_F(TestPredicates, RatingAbove) {
    auto filter = bookdb::RatingAbove(4.0);

    EXPECT_TRUE(filter(book1));
    EXPECT_FALSE(filter(book2));
}

TEST_F(TestPredicates, GenreIs) {
    auto filter = bookdb::GenreIs(Genre::Fiction);

    EXPECT_TRUE(filter(book1));
    EXPECT_FALSE(filter(book2));
}

TEST_F(TestPredicates, AllOf) {
    auto filter = bookdb::all_of(bookdb::YearBetween(2000, 2010), bookdb::RatingAbove(4.0));

    EXPECT_TRUE(filter(book1));
    EXPECT_FALSE(filter(book2));
}

TEST_F(TestPredicates, AnyOf) {
    auto filter = bookdb::any_of(bookdb::YearBetween(2004, 2006), bookdb::RatingAbove(3.5));

    EXPECT_TRUE(filter(book1));
    EXPECT_TRUE(filter(book2));
}

class TestFilterBooks : public ::testing::Test {
public:
    bookdb::BookDatabase<> db{Book{"Title A", "Author A", 2005, Genre::Fiction, 4.5, 100},
                              Book{"Title B", "Author B", 2015, Genre::Mystery, 4.0, 150},
                              Book{"Title C", "Author C", 2010, Genre::SciFi, 3.5, 200},
                              Book{"Title D", "Author D", 2020, Genre::Biography, 4.2, 250}};
};

TEST_F(TestFilterBooks, FilterByYearAndRating) {
    auto filtered =
        bookdb::filterBooks(db.begin(), db.end(), bookdb::YearBetween(2000, 2010), bookdb::RatingAbove(4.0));

    ASSERT_EQ(filtered.size(), 1);
    EXPECT_EQ(filtered[0].get().title, "Title A");
}

TEST_F(TestFilterBooks, FilterSinglePredicate) {
    auto filtered = bookdb::filterBooks(db.begin(), db.end(), bookdb::GenreIs(Genre::Mystery));

    ASSERT_EQ(filtered.size(), 1);
    EXPECT_EQ(filtered[0].get().title, "Title B");
}

TEST_F(TestFilterBooks, FilterNoPredicates) {
    auto filtered = bookdb::filterBooks(db.begin(), db.end());

    ASSERT_EQ(filtered.size(), db.size());
}

TEST_F(TestFilterBooks, FilterEmptyRange) {
    auto filtered = bookdb::filterBooks(db.end(), db.end(), bookdb::GenreIs(Genre::Fiction));

    ASSERT_EQ(filtered.size(), 0);
}