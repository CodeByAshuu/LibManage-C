#ifndef UTILS_H
#define UTILS_H

#include "models.h"

// System and UI
void clearScreen();
void printHeader(const char* title);
void waitForEnter();

// Input
int getIntInput(const char* prompt);
void getStringInput(const char* prompt, char* buffer, int size);
void getPasswordInput(const char* prompt, char* buffer, int maxLen);

// Date Helpers
Date getCurrentDate();
Date addDays(Date date, int days);
int diffDays(Date d1, Date d2); // Returns d1 - d2 in days
char* dateToString(Date d, char* buffer);
int compareDates(Date d1, Date d2); // -1 if d1 < d2, 0 if equal, 1 if d1 > d2

// Security
void maskPassword(char* password); // Simple encryption helper
void unmaskPassword(char* password);

#endif
