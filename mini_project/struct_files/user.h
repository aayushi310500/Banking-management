#ifndef USER_H
#define USER_H
struct User {
    int ID;           // Unique ID for the user
    char name[100];        // User's name
    char email[100];       // User's email or login ID
    char password[100];    // Hashed password
    char login_id[100];
    int role;              // Role of the user ( 1 - Employee, 2 - Manager)
    bool is_active;         // 1 for active, 0 for deactivated users
    char gender;
    int age;
};
#endif