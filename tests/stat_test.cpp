#include "statsistics.hpp"
#include <gtest/gtest.h>

using namespace bookdb;
TEST(TestStatistics, buildAuthorHistogramFlat) {
    BookDatabase<> db{Book{"Title A", "Author A", 2000, Genre::Fiction, 4.5, 100},
                      Book{"Title B", "Author B", 2010, Genre::Mystery, 4.0, 150},
                      Book{"Title C", "Author A", 2020, Genre::SciFi, 4.8, 200},
                      Book{"Title D", "Author A", 2015, Genre::Biography, 4.2, 250}};

    auto histogram = buildAuthorHistogramFlat(db);

    EXPECT_EQ(histogram.size(), 2);
    EXPECT_EQ(histogram.at("Author A"), 3);
    EXPECT_EQ(histogram.at("Author B"), 1);
}

TEST(TestStatistics, calculateGenreRatings) {
    BookDatabase<> db{

        Book{"Title A", "Author A", 2000, Genre::Fiction, 4.5, 100},
        Book{"Title B", "Author B", 2010, Genre::Mystery, 4.0, 150},
        Book{"Title C", "Author C", 2020, Genre::Fiction, 3.5, 200},
        Book{"Title D", "Author D", 2015, Genre::Biography, 4.2, 250},
        Book{"Title E", "Author E", 2018, Genre::Mystery, 4.8, 300},
    };

    auto genreRatings = calculateGenreRatings(db);

    EXPECT_EQ(genreRatings.size(), 3);
    EXPECT_DOUBLE_EQ(genreRatings.at(Genre::Fiction), (4.5 + 3.5) / 2);
    EXPECT_DOUBLE_EQ(genreRatings.at(Genre::Mystery), (4.0 + 4.8) / 2);
    EXPECT_DOUBLE_EQ(genreRatings.at(Genre::Biography), 4.2);
}

TEST(TestStatistics, calculateAverageRating) {
    BookDatabase<> db{Book{"Title A", "Author A", 2000, Genre::Fiction, 4.5, 100},
                      Book{"Title B", "Author B", 2010, Genre::Mystery, 4.0, 150},
                      Book{"Title C", "Author C", 2020, Genre::SciFi, 3.5, 200},
                      Book{"Title D", "Author D", 2015, Genre::Biography, 4.2, 250}};

    double averageRating = calculateAverageRating(db);

    EXPECT_DOUBLE_EQ(averageRating, (4.5 + 4.0 + 3.5 + 4.2) / 4);
}

TEST(TestStatistics, sampleRandomBooks) {
    BookDatabase<> db{

        Book{"Title A", "Author A", 2000, Genre::Fiction, 4.5, 100},
        Book{"Title B", "Author B", 2010, Genre::Mystery, 4.0, 150},
        Book{"Title C", "Author C", 2020, Genre::SciFi, 3.5, 200},
        Book{"Title D", "Author D", 2015, Genre::Biography, 4.2, 250},
        Book{"Title E", "Author E", 2018, Genre::NonFiction, 4.8, 300},
    };

    size_t sample_size = 3;
    auto sampledBooks = sampleRandomBooks(db, sample_size);

    EXPECT_EQ(sampledBooks.size(), sample_size);
}

TEST(TestStatistics, getTopNByRating) {
    BookDatabase<> db{

        Book{"Title A", "Author A", 2000, Genre::Fiction, 4.5, 100},
        Book{"Title B", "Author B", 2010, Genre::Mystery, 4.0, 150},
        Book{"Title C", "Author C", 2020, Genre::SciFi, 3.5, 200},
        Book{"Title D", "Author D", 2015, Genre::Biography, 4.2, 250},
        Book{"Title E", "Author E", 2018, Genre::NonFiction, 4.8, 300},
    };

    size_t n = 2;
    auto topNBooks = getTopNBy(db, n);

    EXPECT_EQ(topNBooks.size(), n);
    EXPECT_DOUBLE_EQ(topNBooks[0].get().rating, 4.8);
    EXPECT_DOUBLE_EQ(topNBooks[1].get().rating, 4.5);
}