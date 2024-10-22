#ifndef ACCOUNT_RECORD
#define ACCOUNT_RECORD

#define MAX_TRANSACTIONS 10

struct Account
{
    int account_number;     
    int customer_id;        
    bool is_active;         
    long int balance;     
    int transactions[MAX_TRANSACTIONS]; 
};

#endif