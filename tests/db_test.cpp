#include "book.hpp"
#include <algorithm>
#include <book_database.hpp>
#include <gtest/gtest.h>
#include <heterogeneous_lookup.hpp>

TEST(TestBookDbContainers, init_list_constructor) {
    bookdb::BookDatabase<> db{
        bookdb::Book{"Title A", "Author A", 2000, bookdb::Genre::Fiction, 4.5, 100},
        bookdb::Book{"Title B", "Author B", 2010, bookdb::Genre::Mystery, 4.0, 150},
    };

    EXPECT_EQ(db.size(), 2);
    EXPECT_EQ(db.GetAuthors().size(), 2);
}

TEST(TestBookDbContainers, push_back_and_emplace_back) {
    bookdb::BookDatabase<> db;

    db.PushBack(bookdb::Book{"Title A", "Author A", 2000, bookdb::Genre::Fiction, 4.5, 100});
    db.PushBack(bookdb::Book{"Title B", "Author B", 2010, bookdb::Genre::Mystery, 4.0, 150});
    db.EmplaceBack("Title C", "Author A", 2020, bookdb::Genre::SciFi, 4.8, 200);

    EXPECT_EQ(db.size(), 3);
    EXPECT_EQ(db.GetAuthors().size(), 2);
}

TEST(TestBookDbContainers, clear_database) {
    bookdb::BookDatabase<> db;

    db.PushBack(bookdb::Book{"Title A", "Author A", 2000, bookdb::Genre::Fiction, 4.5, 100});
    db.PushBack(bookdb::Book{"Title B", "Author B", 2010, bookdb::Genre::Mystery, 4.0, 150});
    db.EmplaceBack("Title C", "Author A", 2020, bookdb::Genre::SciFi, 4.8, 200);

    EXPECT_EQ(db.size(), 3);
    EXPECT_EQ(db.GetAuthors().size(), 2);

    db.Clear();

    EXPECT_EQ(db.size(), 0);
    EXPECT_EQ(db.GetAuthors().size(), 0);
}

TEST(TestBookDbContainers, author_storage_deduplication) {
    bookdb::BookDatabase<> db;

    db.PushBack(bookdb::Book{"Title A", "Author A", 2000, bookdb::Genre::Fiction, 4.5, 100});
    db.PushBack(bookdb::Book{"Title B", "Author A", 2010, bookdb::Genre::Mystery, 4.0, 150});
    db.EmplaceBack("Title C", "Author A", 2020, bookdb::Genre::SciFi, 4.8, 200);

    EXPECT_EQ(db.size(), 3);
    EXPECT_EQ(db.GetAuthors().size(), 1);

    const auto &books = db.GetBooks();
    EXPECT_EQ(books[0].author.data(), books[1].author.data());
    EXPECT_EQ(books[1].author.data(), books[2].author.data());
}

TEST(TestBookDbContainers, heterogeneous_by_author_sorting) {
    bookdb::BookDatabase<> db;

    db.PushBack(bookdb::Book{"Title B", "Author B", 2010, bookdb::Genre::Mystery, 4.0, 150});
    db.PushBack(bookdb::Book{"Title A", "Author A", 2000, bookdb::Genre::Fiction, 4.5, 100});
    db.EmplaceBack("Title C", "Author C", 2020, bookdb::Genre::SciFi, 4.8, 200);

    std::sort(db.begin(), db.end(), bookdb::TransparentCompareBookByAuthor{});

    const auto &books = db.GetBooks();
    EXPECT_EQ(books[0].author, "Author A");
    EXPECT_EQ(books[1].author, "Author B");
    EXPECT_EQ(books[2].author, "Author C");
}

TEST(TestBookDbContainers, book_database_formatting) {
    bookdb::BookDatabase<> db{
        bookdb::Book{"Title A", "Author A", 2000, bookdb::Genre::Fiction, 4.5, 0},
        bookdb::Book{"Title B", "Author B", 2010, bookdb::Genre::Mystery, 4.0, 0},
    };

    std::string formatted = std::format("{}", db);

    std::string expected = "Book Database (size = 2):\n"
                           "Books:\n"
                           "- Title A by Author A (Fiction, 2000) Rating: 4.5 Read Count: 0\n"
                           "- Title B by Author B (Mystery, 2010) Rating: 4.0 Read Count: 0\n"
                           "Authors:\n"
                           "- Author A\n"
                           "- Author B\n";

    EXPECT_EQ(formatted, expected);
}
