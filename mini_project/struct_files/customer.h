#ifndef CUSTOMER_H
#define CUSTOMER_H
struct Customer{
    int ID;
    char name[20];

    char login[30]; 
    char password[30];
    char email[40];
    char gender;
    int age;
    
    // Bank data

    int account_no;
};
#endif