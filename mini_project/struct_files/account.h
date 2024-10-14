#ifndef ACCOUNT_RECORD
#define ACCOUNT_RECORD

#define MAX_TRANSACTIONS 10

struct Account
{
    int account_number;     // 0, 1, 2, ....
    int customer_id;         // Customer IDs
   // bool isRegularAccount; // 1 -> Regular account, 0 -> Joint account
    bool is_active;           // 1 -> Active, 0 -> Deactivated (Deleted)
    long int balance;      // Amount of money in the account
    int transactions[MAX_TRANSACTIONS];  // A list of transaction IDs. Used to look up the transactions. // -1 indicates unused space in array
};

#endif