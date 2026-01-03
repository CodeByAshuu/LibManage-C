#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auth.h"
#include "file_io.h"
#include "utils.h"

int checkAdminExists() {
    User* users;
    int count = readUsers(&users);
    int hasAdmin = 0;
    for (int i = 0; i < count; i++) {
        if (users[i].role == ROLE_ADMIN) {
            hasAdmin = 1;
            break;
        }
    }
    free(users);
    return hasAdmin;
}

// Internal helper to create a user
void performRegistration(UserRole role) {
    User newUser;
    // Generate a simple ID: max ID + 1
    User* users;
    int count = readUsers(&users);
    int maxId = 0;
    for(int i=0; i<count; i++) {
        if(users[i].id > maxId) maxId = users[i].id;
    }
    free(users);
    newUser.id = maxId + 1;
    newUser.role = role;

    printHeader(role == ROLE_ADMIN ? "ADMIN REGISTRATION" : "USER REGISTRATION");
    
    // Username loop
    while(1) {
        getStringInput("Enter Username: ", newUser.username, MAX_USERNAME);
        if (strlen(newUser.username) == 0) continue;
        
        // Check duplicate
        User* existing = findUser(newUser.username);
        if (existing) {
            printf("Username already exists! Try another.\n");
            free(existing);
        } else {
            break;
        }
    }

    getPasswordInput("Enter Password: ", newUser.password, MAX_PASSWORD);
    // Encrypt
    maskPassword(newUser.password);

    appendUser(newUser);
    printf("\nRegistration Successful! ID: %d\n", newUser.id);
    waitForEnter();
}

void registerUser() {
    performRegistration(ROLE_USER);
}

void registerAdmin() {
    // Check if admin exists logic could be here, but for now allow multiple admins or just one
    // But mainly this is called if no admin exists
    performRegistration(ROLE_ADMIN);
}

User* loginProcess(UserRole requiredRole) {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];

    printHeader(requiredRole == ROLE_ADMIN ? "ADMIN LOGIN" : "USER LOGIN");
    
    getStringInput("Username: ", username, MAX_USERNAME);
    getPasswordInput("Password: ", password, MAX_PASSWORD);
    
    // Check user
    User* user = findUser(username);
    if (user != NULL) {
        // Decrypt stored password to compare OR Encrypt input to compare
        // Stored is masked. Input is clear.
        // Let's mask input to match stored.
        maskPassword(password);
        
        if (strcmp(user->password, password) == 0) {
            if (user->role == requiredRole) {
                printf("\nLogin Successful!\n");
                waitForEnter();
                return user;
            } else {
                printf("\nInvalid Role for this login!\n");
            }
        } else {
            printf("\nInvalid Password!\n");
        }
        free(user);
    } else {
        printf("\nUser not found!\n");
    }
    
    waitForEnter();
    return NULL;
}

User* authenticateUser() {
    return loginProcess(ROLE_USER);
}

User* authenticateAdmin() {
    // If no admin exists, prompt to create one first
    if (!checkAdminExists()) {
        printf("No existing admin found. Initializing Admin Setup...\n");
        waitForEnter();
        registerAdmin();
    }
    return loginProcess(ROLE_ADMIN);
}
