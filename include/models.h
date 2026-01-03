#ifndef MODELS_H
#define MODELS_H

#define MAX_TITLE 100
#define MAX_AUTHOR 50
#define MAX_CATEGORY 30
#define MAX_USERNAME 30
#define MAX_PASSWORD 30

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    int id;
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    char category[MAX_CATEGORY];
    int quantity;      // Current available
    int totalQuantity; // Total stock
    int shelfNumber;
} Book;

typedef enum {
    ROLE_ADMIN,
    ROLE_USER
} UserRole;

typedef struct {
    int id;
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD]; // Simple encrypted or hashed in real world
    UserRole role;
} User;

typedef struct {
    int id;
    int bookId;
    int userId;
    Date issueDate;
    Date dueDate;
    Date returnDate; // 0-0-0 if not returned
    int isReturned;  // 0 or 1
    float fine;
} Transaction;

#endif
