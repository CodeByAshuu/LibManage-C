#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "transactions.h"
#include "models.h"
#include "file_io.h"
#include "books.h" // For findBookById, updateBookQty
#include "utils.h"

// Constant for fine per day
#define FINE_PER_DAY 10.0 // Currency units

void issueBookMenu() {
    printHeader("ISSUE BOOK");
    int userId = getIntInput("Enter Student ID: ");
    int bookId = getIntInput("Enter Book ID: ");

    // 1. Validate User
    User* users;
    int userCount = readUsers(&users);
    int validUser = 0;
    for(int i=0; i<userCount; i++) {
        if(users[i].id == userId) {
            validUser = 1;
            break;
        }
    }
    free(users);
    if (!validUser) {
        printf("Invalid User ID!\n");
        waitForEnter();
        return;
    }

    // 2. Validate Book & Stock
    Book* book = findBookById(bookId);
    if (!book) {
        printf("Invalid Book ID!\n");
        waitForEnter();
        return;
    }

    if (book->quantity <= 0) {
        printf("Book is currently unavailable (Out of Stock)!\n");
        free(book);
        waitForEnter();
        return;
    }

    // 3. Prevent duplicate issue (User already has this book?)
    // Optional requirement but good logical practice.
    // "Prevent issuing books if quantity is zero" is checked.
    // Let's assume a user can borrow multiple copies or different books. 
    // Strict logic: One copy of same Book ID per user? Let's implement that.
    
    Transaction* trans;
    int tCount = readTransactions(&trans);
    for(int i=0; i<tCount; i++) {
        if (trans[i].userId == userId && trans[i].bookId == bookId && trans[i].isReturned == 0) {
            printf("User already has this book issued!\n");
            free(trans);
            free(book);
            waitForEnter();
            return;
        }
    }
    
    // 4. Create Transaction
    Transaction newT;
    // ID generation
    int maxId = 0;
    for(int i=0; i<tCount; i++) if(trans[i].id > maxId) maxId = trans[i].id;
    newT.id = maxId + 1;
    free(trans);

    newT.userId = userId;
    newT.bookId = bookId;
    newT.issueDate = getCurrentDate();
    newT.dueDate = addDays(newT.issueDate, 15); // 15 days borrowing period default
    newT.isReturned = 0;
    newT.fine = 0.0;
    // Initialize returnDate to 0
    newT.returnDate.day = 0; newT.returnDate.month = 0; newT.returnDate.year = 0;

    // 5. Update File
    appendTransaction(newT);
    
    // 6. Update Book Stock
    book->quantity--;
    updateBook(*book);
    free(book);

    printf("\nBook Issued Successfully!\n");
    printf("Due Date: %02d-%02d-%04d\n", newT.dueDate.day, newT.dueDate.month, newT.dueDate.year);
    waitForEnter();
}

void returnBookMenu() {
    printHeader("RETURN BOOK");
    int userId = getIntInput("Enter Student ID: ");
    int bookId = getIntInput("Enter Book ID: ");

    Transaction* trans;
    int count = readTransactions(&trans);
    int foundIndex = -1;

    for (int i = 0; i < count; i++) {
        if (trans[i].userId == userId && trans[i].bookId == bookId && trans[i].isReturned == 0) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("No active record found for this User and Book combination.\n");
        free(trans);
        waitForEnter();
        return;
    }

    // Found active transaction
    Transaction* t = &trans[foundIndex];
    t->returnDate = getCurrentDate(); // Assume returned today
    t->isReturned = 1;
    
    // Calculate Fine
    // diffDays(Return, Due)
    // If Return > Due -> Positive diff
    int daysLate = diffDays(t->returnDate, t->dueDate);
    if (daysLate > 0) {
        t->fine = daysLate * FINE_PER_DAY;
        printf("\nBook is Overdue by %d days!\n", daysLate);
        printf("Fine Amount To Pay: %.2f\n", t->fine);
    } else {
        t->fine = 0.0;
        printf("\nReturned on time. No Fine.\n");
    }

    updateTransaction(*t); // Save changes
    free(trans);

    // Update Book Stock (+1)
    updateBookQty(bookId, 1);

    printf("Book Returned Successfully.\n");
    waitForEnter();
}

void viewAllTransactions() {
    printHeader("ALL TRANSACTIONS (Issue History)");
    Transaction* trans;
    int count = readTransactions(&trans);

    if (count == 0) {
        printf("No transactions found.\n");
    } else {
        printf("%-5s | %-10s | %-10s | %-12s | %-12s | %-10s | %-10s\n", 
            "ID", "User", "Book", "Issue Date", "Return Date", "Status", "Fine");
        printf("----------------------------------------------------------------------------------------\n");
        char idate[15], rdate[15];
        for (int i = 0; i < count; i++) {
            dateToString(trans[i].issueDate, idate);
            if (trans[i].isReturned) dateToString(trans[i].returnDate, rdate);
            else strcpy(rdate, "Pending");

            printf("%-5d | %-10d | %-10d | %-12s | %-12s | %-10s | %-10.2f\n", 
                trans[i].id, trans[i].userId, trans[i].bookId, idate, rdate, 
                trans[i].isReturned ? "Returned" : "Issued", trans[i].fine);
        }
    }
    free(trans);
    waitForEnter();
}

void viewMyIssuedBooks(int userId) {
    printHeader("MY ISSUED BOOKS");
    Transaction* trans;
    int count = readTransactions(&trans);
    int found = 0;

    printf("%-5s | %-30s | %-12s | %-12s | %-10s\n", 
            "ID", "Book Title", "Issue Date", "Due Date", "Fine");
    printf("------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        if (trans[i].userId == userId && trans[i].isReturned == 0) {
            Book* b = findBookById(trans[i].bookId);
            char bTitle[MAX_TITLE];
            strcpy(bTitle, b ? b->title : "Unknown Book");
            if(b) free(b);

            char idate[15], ddate[15];
            dateToString(trans[i].issueDate, idate);
            dateToString(trans[i].dueDate, ddate);

            // Calculate current accrued fine estimate if overdue
            float estFine = 0.0;
            Date today = getCurrentDate();
            int daysLate = diffDays(today, trans[i].dueDate);
            if(daysLate > 0) estFine = daysLate * FINE_PER_DAY;

            printf("%-5d | %-30s | %-12s | %-12s | %-10.2f\n", 
                trans[i].bookId, bTitle, idate, ddate, estFine);
            
            found = 1;
        }
    }
    
    if (!found) printf("No currently issued books.\n");
    free(trans);
    waitForEnter();
}
