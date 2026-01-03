#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h> // For getch() on Windows
#include "utils.h"

void clearScreen() {
    system("cls");
}

void printHeader(const char* title) {
    clearScreen();
    printf("==================================================\n");
    printf("   LIBRARY MANAGEMENT SYSTEM\n");
    printf("   %s\n", title);
    printf("==================================================\n");
}

void waitForEnter() {
    printf("\nPress Enter to continue...");
    // Clear any remaining input and wait for a final Enter
    // Since we'll use fgets everywhere, there shouldn't be much left,
    // but this handles stray characters.
    int c;
    while ((c = getchar()) != '\n' && c != EOF && c != 0);
}

int getIntInput(const char* prompt) {
    char buffer[100];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &value) == 1) {
                return value;
            }
        }
        printf("Invalid input. Please enter a number.\n");
    }
}

void getStringInput(const char* prompt, char* buffer, int size) {
    if (prompt && strlen(prompt) > 0) {
        printf("%s", prompt);
    }
    
    if (fgets(buffer, size, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
    }
}

void getPasswordInput(const char* prompt, char* buffer, int maxLen) {
    printf("%s", prompt);
    int i = 0;
    char ch;
    while (1) {
        ch = getch();
        if (ch == 13) { // Enter
            buffer[i] = '\0';
            break;
        } else if (ch == 8) { // Backspace
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else if (i < maxLen - 1) {
            buffer[i++] = ch;
            printf("*");
        }
    }
    printf("\n");
}

// Date helpers
Date getCurrentDate() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    Date d;
    d.day = tm.tm_mday;
    d.month = tm.tm_mon + 1;
    d.year = tm.tm_year + 1900;
    return d;
}

// Very basic date logic (simplification: 30 days per month)
// For academic project precision, this is often acceptable unless strict calendar is required.
// But let's try to be slightly improved.
int isLeap(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

int daysInMonth(int m, int y) {
    if (m == 2) return isLeap(y) ? 29 : 28;
    if (m == 4 || m == 6 || m == 9 || m == 11) return 30;
    return 31;
}

Date addDays(Date date, int days) {
    Date newDate = date;
    newDate.day += days;
    while (newDate.day > daysInMonth(newDate.month, newDate.year)) {
        newDate.day -= daysInMonth(newDate.month, newDate.year);
        newDate.month++;
        if (newDate.month > 12) {
            newDate.month = 1;
            newDate.year++;
        }
    }
    return newDate;
}

// Basic diff, ignoring leap years for simplicity if complicated, but let's do a simple epoch approx
long dateToAbsDays(Date d) {
    // Rata Die algorithm or similar is good, but let's stick to a simple accumulation
    long total = 0;
    for (int y = 0; y < d.year; y++) {
        total += isLeap(y) ? 366 : 365;
    }
    for (int m = 1; m < d.month; m++) {
        total += daysInMonth(m, d.year);
    }
    total += d.day;
    return total;
}

int diffDays(Date d1, Date d2) {
    // d1 - d2
    return (int)(dateToAbsDays(d1) - dateToAbsDays(d2));
}

char* dateToString(Date d, char* buffer) {
    sprintf(buffer, "%02d-%02d-%04d", d.day, d.month, d.year);
    return buffer;
}

int compareDates(Date d1, Date d2) {
    long days1 = dateToAbsDays(d1);
    long days2 = dateToAbsDays(d2);
    if (days1 < days2) return -1;
    if (days1 > days2) return 1;
    return 0;
}

// Security
void maskPassword(char* password) {
    // XOR Encryption for demo
    char key = 'K'; 
    for (int i = 0; password[i] != '\0'; i++) {
        password[i] ^= key;
    }
}

void unmaskPassword(char* password) {
    maskPassword(password); // Symmetric
}
