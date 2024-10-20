#ifndef MANAGER_FUN_H
#define MANAGER_FUN_H

#include <stdio.h>     // Import for `printf` & `perror`
#include <unistd.h>    // Import for `read`, `write & `lseek`
#include <string.h>    // Import for string functions
#include <stdbool.h>   // Import for `bool` data type
#include <sys/types.h> // Import for `open`, `lseek`
#include <sys/stat.h>  // Import for `open`
#include <fcntl.h>     // Import for `open`
#include <stdlib.h>    // Import for `atoi`
#include <errno.h>     // Import for `errno`
// #include <sodium.h>
#include <sys/sem.h>

#include <openssl/evp.h>
#include <openssl/sha.h>

#define MAX_LINE 1024
#define HASH_HEX_SIZE 64

#include "login_user.h"
#include "../common_functions/common_fun.h"
#include "../struct_files/customer.h"
#include "../struct_files/account.h"
#include "../struct_files/feedback.h"
#include "../common_functions/hash_password.h"
#include "../struct_files/loan.h"

bool activate_deactivate_user_(int connection_fd)
{
    int acc_id;
    char write_buffer[2000], read_buffer[2000];
    ssize_t wb, rb;
    struct Account account;

    // Ask for user ID
    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));
    wb = write(connection_fd, "Enter Account ID to activate/deactivate: ", strlen("Enter User ID to activate/deactivate: "));
    if (wb == -1)
    {
        perror("Error writing to connection_fd");
        return false;
    }
    rb = read(connection_fd, read_buffer, sizeof(read_buffer));
    if (rb == -1)
    {
        perror("Error reading user ID");
        return false;
    }
    acc_id = atoi(read_buffer); // Convert input to integer

    // Search user by ID in the file (you should load all users and search by ID)
    int fd = open(ACCOUNT_FILE, O_RDWR);
    if (fd == -1)
    {
        perror("Error opening user file");
        return false;
    }

    bool account_found = false;

    while (read(fd, &account, sizeof(struct Account)) > 0)
    {

        if (account.account_number == acc_id)
        {
            account_found = true;
            // Toggle activation status
            account.is_active = !account.is_active;
            lseek(fd, -sizeof(struct Account), SEEK_CUR);

            if (write(fd, &account, sizeof(struct Account)) == -1)
            {
                perror("Error updating user status");
                close(fd);
                return false;
            }
            break;
        }
    }

    if (!account_found)
    {
        bzero(write_buffer, sizeof(write_buffer));
        bzero(read_buffer, sizeof(read_buffer));
        wb = write(connection_fd, "User account not found^", strlen("User account not found^"));
        rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read
        close(fd);
        return false;
    }
    close(fd);
    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));

    wb = write(connection_fd, "User account status updated.\n", strlen("User account status updated.\n"));
    // wb = write(connection_fd, "User role updated successfully.\n", strlen("User role updated successfully.\n"));

    // sprintf(write_buffer, "%s%d", ADMIN_ADD_ACCOUNT_NUMBER, curr_account.account_number);
    strcat(write_buffer, "\nRedirecting you to the main menu ...^");
    wb = write(connection_fd, write_buffer, strlen(write_buffer));
    rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read
    return true;
}

bool assign_loan_to_employee(int connection_fd)
{
    struct Loan loan;
    struct User user;
    int loan_id_, employee_id;
    char write_buffer[2000], read_buffer[2000];
    ssize_t wb, rb;
    int fd1 = open(LOAN_FILE, O_RDWR); // Open loan file for reading and writing

    if (fd1 == -1)
    {
        perror("Error opening loan file");
        return false;
    }

    int fd2 = open(USER_FILE, O_RDONLY);
    if (fd1 == -1)
    {
        perror("Error opening loan file");
        return false;
    }

    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));
    sprintf(write_buffer, "%s", ENTER_LOAN_ID);
    wb = write(connection_fd, write_buffer, strlen(write_buffer)); // change
    if (wb == -1)
    {
        perror("Error writing ENTER_LOAN_ID message to client!");
        return false;
    }

    rb = read(connection_fd, read_buffer, sizeof(read_buffer));
    if (rb == -1)
    {
        perror("Error reading ENTER_LOAN_ID response from client!");
        return false;
    }
    read_buffer[rb] = '\0';

    // printf("readbuff : %s", read_buffer);
    loan_id_ = atoi(read_buffer);

    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));
    sprintf(write_buffer, "%s", ENTER_EMPLOYEE_ID);
    wb = write(connection_fd, write_buffer, strlen(write_buffer)); // change
    if (wb == -1)
    {
        perror("Error writing ENTER_EMPLOYEE_ID message to client!");
        return false;
    }

    rb = read(connection_fd, read_buffer, sizeof(read_buffer));
    if (rb == -1)
    {
        perror("Error reading ENTER_EMPLOYEE_ID response from client!");
        return false;
    }
    read_buffer[rb] = '\0';

    // printf("readbuff : %s", read_buffer);
    employee_id = atoi(read_buffer);

    bool loan_found = false;
    while (read(fd1, &loan, sizeof(struct Loan)) > 0)
    {
        if (loan.loan_id == loan_id_)
        { // Check if loan is pending
            loan_found = true;
            // Assign the loan to the employee

            // printf("Loan ID %d assigned to Employee ID %s successfully.\n", loan_id, employee_id);
            break;
        }
    }

    if (!loan_found)
    {
        bzero(write_buffer, sizeof(write_buffer));
        bzero(read_buffer, sizeof(read_buffer));
        sprintf(write_buffer, "%s", LOAN_ID_NOTFOUND);
        wb = write(connection_fd, write_buffer, strlen(write_buffer)); // change
        if (wb == -1)
        {
            perror("Error writing LOAN_ID_NOTFOUND message to client!");
            return false;
        }
        rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read
        // printf("Loan ID %d not found or already assigned.\n", loan_id);
    }
    bool employee_found = false;
    while (read(fd2, &user, sizeof(struct User)) > 0)
    {

        if (user.ID = employee_id && user.role == 1)
        { // Check if loan is pending
            employee_found = true;
            // Assign the loan to the employee
            // printf("Loan ID %d assigned to Employee ID %s successfully.\n", loan_id, employee_id);
            break;
        }
    }
    close(fd2);

    if (!employee_found)
    {
        bzero(write_buffer, sizeof(write_buffer));
        bzero(read_buffer, sizeof(read_buffer));
        sprintf(write_buffer, "%s", EMPLOYEE_ID_NOTFOUND);
        wb = write(connection_fd, write_buffer, strlen(write_buffer)); // change
        if (wb == -1)
        {
            perror("Error writing EMPLOYEE_ID_NOTFOUND message to client!");
            return false;
        }
        rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read
        // printf("Loan ID %d not found or already assigned.\n", loan_id);
    }

    loan.employee_id = employee_id;

    // lseek(fd1, -sizeof(struct Loan), SEEK_CUR);
    //  strcpy(loan.status ); // Change status to assigned
    lseek(fd1, 0, SEEK_SET);
    while (read(fd1, &loan, sizeof(struct Loan)) > 0)
    {
        if (loan.loan_id == loan_id_)
        { // Check if loan is pending
            if (write(fd1, &loan, sizeof(struct Loan)) == -1)
            {
                perror("Error updating loan status");
                close(fd1);
                return false;
            }
            // Assign the loan to the employee

            // printf("Loan ID %d assigned to Employee ID %s successfully.\n", loan_id, employee_id);
            break;
        }
    }
    // Move file pointer to the position of the loan record

    sprintf(write_buffer, "%s", LOAN_ID_ASSIGN_SUCCESS);
    wb = write(connection_fd, write_buffer, strlen(write_buffer)); // change
    if (wb == -1)
    {
        perror("Error writing LOAN_ID_ASSIGN_SUCCESS message to client!");
        return false;
    }
    rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read

    return loan_found;
}

bool review_customer_feedbacks(int connection_fd)
{
    struct Feedback feedback;
    char write_buffer[3000], read_buffer[3000];
    ssize_t wb;
    int fd = open(FEEDBACK_FILE, O_RDWR); // Open feedback file for reading

    if (fd == -1)
    {
        perror("Error opening feedback file");
        return false;
    }

    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));
    bool feedback_found = false;

    // Read through the feedback records
    lseek(fd, 0, SEEK_SET);
    while (read(fd, &feedback, sizeof(struct Feedback)) > 0)
    {
        bzero(write_buffer, sizeof(write_buffer));
        bzero(read_buffer, sizeof(read_buffer));

        // At least one feedback found
        if (feedback.is_reviewed == false)
        {
            feedback.is_reviewed = true;
            feedback_found = true;

            if (write(fd, &feedback, sizeof(struct Feedback)) == -1)
            {
                perror("Error updating loan status");
                close(fd);
                return false;
            }
            snprintf(write_buffer, sizeof(write_buffer), "\nFeedback ID: %d\nCustomer ID: %d\nFeedback Text: %s",
                     feedback.feedback_id, feedback.customer_id, feedback.feedback_text);
            strcat(write_buffer, "^");

            // Send the feedback to the manager over the socket
            wb = write(connection_fd, write_buffer, strlen(write_buffer));
            if (wb == -1)
            {
                perror("Error writing feedback message to client!");
                close(fd);
                return false;
            }
            int rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read
        }

        // Prepare the feedback for sending
    }

    if (!feedback_found)
    {
        // If no feedback was found, notify the manager
        snprintf(write_buffer, sizeof(write_buffer), "No customer feedback found to be reviewed. ^");
        wb = write(connection_fd, write_buffer, strlen(write_buffer));
        if (wb == -1)
        {
            perror("Error writing no feedback message to client!");
            close(fd);
            return false;
        }
        int rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read
    }

    close(fd); // Close the feedback file
    return true;
}

bool view_account_detail(int connection_fd)
{
    int acc_id;
    char write_buffer[2000], read_buffer[2000];
    ssize_t wb, rb;
    struct Account account;

    // Ask for user ID
    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));
    wb = write(connection_fd, "Enter Account ID : ", strlen("Enter Account ID : "));
    if (wb == -1)
    {
        perror("Error writing to connection_fd");
        return false;
    }
    rb = read(connection_fd, read_buffer, sizeof(read_buffer));
    if (rb == -1)
    {
        perror("Error reading user ID");
        return false;
    }
    acc_id = atoi(read_buffer); // Convert input to integer

    // Search user by ID in the file (you should load all users and search by ID)
    int fd = open(ACCOUNT_FILE, O_RDWR);
    if (fd == -1)
    {
        perror("Error opening user file");
        return false;
    }

    bool account_found = false;

    while (read(fd, &account, sizeof(struct Account)) > 0)
    {
        if (account.account_number == acc_id)
        {
            account_found = true;
            // Toggle activation statu
            break;
        }
    }
    close(fd);
    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));
    if (!account_found)
    {
        wb = write(connection_fd, "Account does not exist ^", strlen("Account does not exist ^"));
        if (wb == -1)
        {
            perror("Error writing to connection_fd");
            return false;
        }
        read(connection_fd, read_buffer, sizeof(read_buffer));
        return false;
    }

    snprintf(write_buffer, sizeof(write_buffer), "\nAccount ID: %d\nCustomer ID: %d\n is_active: %d \n Balance: %ld",
             account.account_number, account.customer_id, account.is_active, account.balance);
    strcat(write_buffer, "^");

    // Send the feedback to the manager over the socket
    wb = write(connection_fd, write_buffer, strlen(write_buffer));
    if (wb == -1)
    {
        perror("Error writing feedback message to client!");
        close(fd);
        return false;
    }
    read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read

    return true;
}

bool view_loan_detais(int connection_fd)
{
    struct Loan loan;
    int loan_id_, employee_id;
    char write_buffer[2000], read_buffer[2000];
    ssize_t wb, rb;
    int fd = open(LOAN_FILE, O_RDWR); // Open loan file for reading and writing

    if (fd == -1)
    {
        perror("Error opening loan file");
        return false;
    }
    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));
    sprintf(write_buffer, "%s", ENTER_LOAN_ID);
    wb = write(connection_fd, write_buffer, strlen(write_buffer)); // change
    if (wb == -1)
    {
        perror("Error writing ENTER_LOAN_ID message to client!");
        return false;
    }

    rb = read(connection_fd, read_buffer, sizeof(read_buffer));
    if (rb == -1)
    {
        perror("Error reading ENTER_LOAN_ID response from client!");
        return false;
    }
    read_buffer[rb] = '\0';

    // printf("readbuff : %s", read_buffer);
    loan_id_ = atoi(read_buffer);

    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));
    bool loan_found = false;
    while (read(fd, &loan, sizeof(struct Loan)) > 0)
    {
        if (loan.loan_id == loan_id_)
        { // Check if loan is pending
            loan_found = true;
            break;
        }
    }
    if (!loan_found)
    {
        wb = write(connection_fd, "Data for given loan id does not exist ^", strlen("Data for given loan id does not exist ^"));
        if (wb == -1)
        {
            perror("Error writing to connection_fd");
            return false;
        }
        read(connection_fd, read_buffer, sizeof(read_buffer));
        return false;
    }
    snprintf(write_buffer, sizeof(write_buffer), "\nLoan ID: %d\nCustomer ID: %d\nAmount: %f \nDuration: %d \nInterest Rate: %f \nStatus: %s \n Empoyee id assigned to: %d",
             loan.loan_id, loan.customer_id, loan.amount, loan.duration, loan.interest_rate, loan.status, loan.employee_id);
    strcat(write_buffer, "^");

    // Send the feedback to the manager over the socket
    wb = write(connection_fd, write_buffer, strlen(write_buffer));
    if (wb == -1)
    {
        perror("Error writing feedback message to client!");
        close(fd);
        return false;
    }
    read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read
    return true;
}

#endif