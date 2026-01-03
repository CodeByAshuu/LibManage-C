#include <stdio.h>
#include <stdlib.h>
#include "auth.h"
#include "books.h"
#include "transactions.h"
#include "file_io.h"
#include "utils.h"
#include "models.h"

void adminDashboard(User* admin) {
    int choice;
    do {
        printHeader("ADMIN DASHBOARD");
        printf("Logged in as: %s\n\n", admin->username);
        printf("1. Add New Book\n");
        printf("2. View All Books\n");
        printf("3. Search Books\n");
        printf("4. Update Book\n");
        printf("5. Delete Book\n");
        printf("6. Issue Book\n");
        printf("7. Return Book\n");
        printf("8. View Issued/Transaction History\n");
        printf("9. Register New Admin (Setup)\n");
        printf("10. Backup Data\n");
        printf("0. Logout\n");
        
        choice = getIntInput("\nEnter Choice: ");

        switch(choice) {
            case 1: addBookMenu(); break;
            case 2: viewBooksMenu(); break;
            case 3: searchBooksMenu(); break;
            case 4: updateBookMenu(); break;
            case 5: deleteBookMenu(); break;
            case 6: issueBookMenu(); break;
            case 7: returnBookMenu(); break;
            case 8: viewAllTransactions(); break;
            case 9: registerAdmin(); break;
            case 10: backupData(); waitForEnter(); break;
            case 0: printf("Logging out...\n"); break;
            default: printf("Invalid choice!\n"); waitForEnter();
        }
    } while (choice != 0);
}

void userDashboard(User* user) {
    int choice;
    do {
        printHeader("STUDENT DASHBOARD");
        printf("Welcome, %s (ID: %d)\n\n", user->username, user->id);
        printf("1. View Available Books\n");
        printf("2. Search Books\n");
        printf("3. View My Issued Books / Check Fines\n");
        printf("0. Logout\n");

        choice = getIntInput("\nEnter Choice: ");

        switch(choice) {
            case 1: viewBooksMenu(); break; // Reusing same view for now
            case 2: searchBooksMenu(); break;
            case 3: viewMyIssuedBooks(user->id); break;
            case 0: printf("Logging out...\n"); break;
            default: printf("Invalid choice!\n"); waitForEnter();
        }
    } while (choice != 0);
}

int main() {
    initDataFiles();
    
    int choice;
    do {
        printHeader("TOWN LIBRARY SYSTEM");
        printf("1. Admin Login\n");
        printf("2. User Login\n");
        printf("3. New User Registration\n");
        printf("0. Exit\n");
        
        choice = getIntInput("\nEnter Choice: ");
        
        if (choice == 1) {
            User* admin = authenticateAdmin();
            if (admin) {
                adminDashboard(admin);
                free(admin);
            }
        } else if (choice == 2) {
            User* user = authenticateUser();
            if (user) {
                userDashboard(user);
                free(user);
            }
        } else if (choice == 3) {
            registerUser();
        } else if (choice == 0) {
            printf("Exiting... Goodbye!\n");
        } else {
            printf("Invalid Choice.\n");
            waitForEnter();
        }
    } while (choice != 0);

    return 0;
}
