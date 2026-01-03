#ifndef FILE_IO_H
#define FILE_IO_H

#include "models.h"

// Constants for file paths
#define FILE_BOOKS "data/books.dat"
#define FILE_USERS "data/users.dat"
#define FILE_TRANSACTIONS "data/transactions.dat"

// Initialization
void initDataFiles();
void backupData();

// Book IO
int readBooks(Book** books); // Returns count, allocates memory
void writeBooks(Book* books, int count);
void appendBook(Book book);
void updateBook(Book book); // Uses ID to find and update
void saveBookList(Book* books, int count); // Overwrite all

// User IO
int readUsers(User** users);
void writeUsers(User* users, int count);
void appendUser(User user);
User* findUser(const char* username);

// Transaction IO
int readTransactions(Transaction** transactions);
void writeTransactions(Transaction* transactions, int count);
void appendTransaction(Transaction t);
void updateTransaction(Transaction t);

#endif
