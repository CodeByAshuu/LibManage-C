#ifndef BOOKS_H
#define BOOKS_H

#include "models.h"

void addBookMenu();
void viewBooksMenu();
void searchBooksMenu();
void deleteBookMenu();
void updateBookMenu();

// Internal helpers potentially exposed for transaction module
Book* findBookById(int id);
void updateBookQty(int id, int delta); // +1 or -1

#endif
