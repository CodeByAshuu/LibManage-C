#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include "models.h"

void issueBookMenu();
void returnBookMenu();
void viewMyIssuedBooks(int userId);
void viewAllTransactions(); // Admin only

#endif
