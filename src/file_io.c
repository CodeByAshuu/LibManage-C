#include <stdio.h>
#include <stdlib.h>
#include <io.h> // For _access
#include "file_io.h"
#include <string.h>

void ensureDirectory(const char* path) {
    // Windows implementation to check 'data' folder
    // For simplicity, we assume 'data' folder exists or we created it manually as per task list
}

void initDataFiles() {
    FILE *fp;
    
    // Check Books File
    fp = fopen(FILE_BOOKS, "rb");
    if (fp == NULL) {
        fp = fopen(FILE_BOOKS, "wb"); // Create if not exists
        if (fp) fclose(fp);
    } else fclose(fp);

    // Check Users File
    fp = fopen(FILE_USERS, "rb");
    if (fp == NULL) {
        fp = fopen(FILE_USERS, "wb");
        if (fp) fclose(fp);
    } else fclose(fp);

    // Check Transactions File
    fp = fopen(FILE_TRANSACTIONS, "rb");
    if (fp == NULL) {
        fp = fopen(FILE_TRANSACTIONS, "wb");
        if (fp) fclose(fp);
    } else fclose(fp);
}

void copyFile(const char* src, const char* dest) {
    FILE *source = fopen(src, "rb");
    if (source == NULL) return;

    FILE *destination = fopen(dest, "wb");
    if (destination == NULL) {
        fclose(source);
        return;
    }

    char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        fwrite(buffer, 1, bytes, destination);
    }

    fclose(source);
    fclose(destination);
}

void backupData() {
    // Ensure backup directory exists (using system command for simplicity on Windows)
    system("if not exist backup mkdir backup");
    
    copyFile(FILE_BOOKS, "backup/books.dat.bak");
    copyFile(FILE_USERS, "backup/users.dat.bak");
    copyFile(FILE_TRANSACTIONS, "backup/transactions.dat.bak");
    
    printf("Backup completed successfully in 'backup/' directory.\n");
}

// --- BOOK IO ---

int readBooks(Book** books) {
    FILE *fp = fopen(FILE_BOOKS, "rb");
    if (!fp) return 0;
    
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    int count = size / sizeof(Book);
    *books = (Book*)malloc(count * sizeof(Book));
    
    fread(*books, sizeof(Book), count, fp);
    fclose(fp);
    return count;
}

void writeBooks(Book* books, int count) {
    FILE *fp = fopen(FILE_BOOKS, "wb");
    if (!fp) return;
    fwrite(books, sizeof(Book), count, fp);
    fclose(fp);
}

void appendBook(Book book) {
    FILE *fp = fopen(FILE_BOOKS, "ab");
    if (!fp) return;
    fwrite(&book, sizeof(Book), 1, fp);
    fclose(fp);
}

void updateBook(Book book) {
    // This is inefficient for large files, but for a project it works.
    // Better: Read all, update, write all. Or fseek to position.
    // Let's do Read All -> Update -> Write All for simplicity and safety.
    Book* books;
    int count = readBooks(&books);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (books[i].id == book.id) {
            books[i] = book;
            found = 1;
            break;
        }
    }
    if (found) {
        writeBooks(books, count);
    }
    free(books);
}

// --- USER IO ---

int readUsers(User** users) {
    FILE *fp = fopen(FILE_USERS, "rb");
    if (!fp) return 0;
    
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    int count = size / sizeof(User);
    *users = (User*)malloc(count * sizeof(User));
    
    fread(*users, sizeof(User), count, fp);
    fclose(fp);
    return count;
}

void writeUsers(User* users, int count) {
    FILE *fp = fopen(FILE_USERS, "wb");
    if (!fp) return;
    fwrite(users, sizeof(User), count, fp);
    fclose(fp);
}

void appendUser(User user) {
    FILE *fp = fopen(FILE_USERS, "ab");
    if (!fp) return;
    fwrite(&user, sizeof(User), 1, fp);
    fclose(fp);
}

User* findUser(const char* username) {
    // Returns a pointer to a malloc'd user or NULL
    User* users;
    int count = readUsers(&users);
    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            User* found = (User*)malloc(sizeof(User));
            *found = users[i];
            free(users);
            return found;
        }
    }
    free(users);
    return NULL;
}

// --- TRANSACTION IO ---

int readTransactions(Transaction** transactions) {
    FILE *fp = fopen(FILE_TRANSACTIONS, "rb");
    if (!fp) return 0;
    
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    int count = size / sizeof(Transaction);
    *transactions = (Transaction*)malloc(count * sizeof(Transaction));
    
    fread(*transactions, sizeof(Transaction), count, fp);
    fclose(fp);
    return count;
}

void writeTransactions(Transaction* transactions, int count) {
    FILE *fp = fopen(FILE_TRANSACTIONS, "wb");
    if (!fp) return;
    fwrite(transactions, sizeof(Transaction), count, fp);
    fclose(fp);
}

void appendTransaction(Transaction t) {
    FILE *fp = fopen(FILE_TRANSACTIONS, "ab");
    if (!fp) return;
    fwrite(&t, sizeof(Transaction), 1, fp);
    fclose(fp);
}

void updateTransaction(Transaction t) {
    Transaction* transactions;
    int count = readTransactions(&transactions);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (transactions[i].id == t.id) {
            transactions[i] = t;
            found = 1;
            break;
        }
    }
    if (found) {
        writeTransactions(transactions, count);
    }
    free(transactions);
}
