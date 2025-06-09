#include "book_database.hpp"
#include <gtest/gtest.h>

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