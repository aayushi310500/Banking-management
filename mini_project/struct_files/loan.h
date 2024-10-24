#ifndef LOAN_H
#define LOAN_H
struct Loan {
    int loan_id;
    int customer_id;
    double amount;
    int duration; // in months
    double interest_rate;
    char status[20]; // "Pending", "Approved", "Rejected"
    int employee_id;
};
#endif