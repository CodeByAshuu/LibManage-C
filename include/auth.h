#ifndef AUTH_H
#define AUTH_H

#include "models.h"

// Returns a pointer to the logged-in User or NULL if failed
// If ID is needed session persistence, we can return the ID or struct.
// For this simple app, returning a dynamically allocated User copy is fine, caller must free.
User* authenticateUser();
User* authenticateAdmin();

void registerUser();
void registerAdmin(); // Only generic for setup, usually disabled or restricted

#endif
