#include "book.hpp"
#include "book_database.hpp"
#include <gtest/gtest.h>
#include <iostream>

#include "filters.hpp"
#include "statsistics.hpp"

TEST(BookDatsabaseTest, TestPush) {
    bookdb::BookDatabase db;
    bookdb::Book book{"1984", "George Orwell", 1949, bookdb::Genre::SciFi, 4., 190};
    db.PushBack(book);
    EXPECT_EQ(db.size(), 1);
}

TEST(BookDatsabaseTest, TestEmplace) {
    bookdb::BookDatabase db;
    db.EmplaceBack("1984", "George Orwell", 1949, bookdb::Genre::SciFi, 4., 190);
    EXPECT_EQ(db.size(), 1);
}

TEST(BookDatsabaseTest, TestGetAuthors) {
    bookdb::BookDatabase db;
    db.EmplaceBack("1984", "George Orwell", 1949, bookdb::Genre::SciFi, 4., 190);
    db.EmplaceBack("Lord of the Flies", "William Golding", 1954, bookdb::Genre::Fiction, 4.2, 89);
    auto db_authors = db.GetAuthors();
    EXPECT_TRUE(db_authors.contains("George Orwell") && db_authors.contains("William Golding"));
}

TEST(BookDatsabaseTest, TestRating) {
    bookdb::BookDatabase db;
    db.EmplaceBack("1984", "George Orwell", 1949, bookdb::Genre::SciFi, 2., 190);
    db.EmplaceBack("Lord of the Flies", "William Golding", 1954, bookdb::Genre::Fiction, 3., 89);
    db.EmplaceBack("The Catcher in the Rye", "J.D. Salinger", 1951, bookdb::Genre::Fiction, 10., 112);
    EXPECT_DOUBLE_EQ(calculateAverageRating(db), 5.);
}

TEST(BookDatsabaseTest, TestGetTopN) {
    bookdb::BookDatabase db;
    db.EmplaceBack("1984", "George Orwell", 1949, bookdb::Genre::SciFi, 2., 190);
    db.EmplaceBack("Lord of the Flies", "William Golding", 1954, bookdb::Genre::Fiction, 3., 89);
    db.EmplaceBack("The Catcher in the Rye", "J.D. Salinger", 1951, bookdb::Genre::Fiction, 10., 112);
    db.EmplaceBack("The Hobbit", "J.R.R. Tolkien", 1937, bookdb::Genre::Fiction, 4.9, 203);
    auto top_N = getTopNBy(db, 2);
    EXPECT_EQ(top_N.size(), 2);
    EXPECT_EQ(top_N.begin()->get().title, "The Catcher in the Rye");
    EXPECT_EQ((top_N.begin() + 1)->get().title, "The Hobbit");
}

TEST(BookDatsabaseTest, TestFilter) {
    bookdb::BookDatabase db;
    db.EmplaceBack("1984", "George Orwell", 1949, bookdb::Genre::SciFi, 7., 190);
    db.EmplaceBack("Lord of the Flies", "William Golding", 1954, bookdb::Genre::Fiction, 3., 89);
    db.EmplaceBack("The Catcher in the Rye", "J.D. Salinger", 1951, bookdb::Genre::Fiction, 10., 112);
    db.EmplaceBack("The Hobbit", "J.R.R. Tolkien", 1937, bookdb::Genre::Fiction, 4.9, 203);
    auto filtered = filterBooks(db.begin(), db.end(), all_of(RatingAbove(4.9), GenreIs(bookdb::Genre::Fiction)));
    EXPECT_EQ(filtered.size(), 2);
    EXPECT_EQ(filtered.begin()->get().title, "The Catcher in the Rye");
    EXPECT_EQ((filtered.begin() + 1)->get().title, "The Hobbit");
}

TEST(BookDatsabaseTest, TestGetEmptyDb) {
    bookdb::BookDatabase db;
    auto authors = db.GetAuthors();
    EXPECT_EQ(authors.size(), 0);
}

TEST(BookDatsabaseTest, TestFilterEmptyDb) {
    bookdb::BookDatabase db;
    auto filtered = filterBooks(db.begin(), db.end(), all_of(RatingAbove(4.9), GenreIs(bookdb::Genre::Fiction)));
    EXPECT_EQ(filtered.size(), 0);
}