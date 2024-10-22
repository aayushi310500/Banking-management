#ifndef USER_H
#define USER_H
struct User {
    int ID;           
    char name[100];        
    char email[100];      
    char password[100];   
    char login_id[100];
    int role;             
    bool is_active;         
    char gender;
    int age;
};
#endif