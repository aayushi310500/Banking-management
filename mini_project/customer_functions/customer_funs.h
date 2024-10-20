#ifndef CUSTOMER_FUNCTIONS_H
#define CUSTOMER_FUNCTIONS_H

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

#include "login_customer.h"
#include "../common_functions/common_fun.h"
#include "../struct_files/customer.h"
#include "../struct_files/feedback.h"
#include "../common_functions/hash_password.h"
#include "../struct_files/loan.h"

bool get_balance(int connection_fd);
bool deposit_money(int connection_fd);
bool withdraw_money(int connection_fd);
bool lock_critical_section(struct sembuf *semOp);
bool unlock_critical_section(struct sembuf *semOp);
bool transfer_funds(int connection_fd);
bool adding_feedback(int connection_fd);
bool apply_for_loan(int connection_fd);
int generate_new_loan_id();
double assign_interest_rate(double amount, int duration);
bool find_and_update_account_in_file(int fd, long int account_number, struct Account *account, int balance);
void lock_account(int fd, long int account_number);
void unlock_account(int fd, long int account_number);
bool get_transaction_details(int connection_fd, int account_num);
int write_transaction_to_file(int accountNumber, long int oldBalance, long int newBalance, bool operation);
void write_transaction_to_array(int *transactionArray, int ID);

struct Customer loggedInCustomer;
int sem_id;

bool customer_operation_handler(int connection_fd)
{
    // Assume `loggedInCustomer` is properly declared and initialized elsewhere
    struct Customer loggedInCustomer;

    // Attempt to log in the customer
    bool ans = login_handler_customer(connection_fd, &loggedInCustomer);
    if (ans)
    {
        char read_buffer[1000], write_buffer[1000];
        ssize_t rb, wb;
        int choice;

        key_t semKey = ftok(CUSTOMER_FILE, loggedInCustomer.account_no); // Generate a key based on the account number hence, different customers will have different semaphores

        union semun
        {
            int val; // Value of the semaphore
        } semSet;

        int semctlStatus;
        sem_id = semget(semKey, 1, 0); // Get the semaphore if it exists
        if (sem_id == -1)
        {
            sem_id = semget(semKey, 1, IPC_CREAT | 0700); // Create a new semaphore
            if (sem_id == -1)
            {
                perror("Error while creating semaphore!");
                _exit(1);
            }

            semSet.val = 1; // Set a binary semaphore
            semctlStatus = semctl(sem_id, 0, SETVAL, semSet);
            if (semctlStatus == -1)
            {
                perror("Error while initializing a binary sempahore!");
                _exit(1);
            }
        }
        //  bzero(write_buffer, sizeof(write_buffer));
        bzero(write_buffer, sizeof(write_buffer));
        strcpy(write_buffer, CUSTOMER_LOGIN_SUCCESS);
        while (1)
        {
            strcat(write_buffer, "\n");
            strcat(write_buffer, CUSTOMER_MENU);
            // printf("\nCustomer menu\n");
            wb = write(connection_fd, write_buffer, strlen(write_buffer));
            if (wb == -1)
            {
                perror("Error while sending menu to the customer");
                return false;
            }
            bzero(write_buffer, sizeof(write_buffer));
            bzero(read_buffer, sizeof(read_buffer)); // Clear buffer

            // bzero(write_buffer, sizeof(write_buffer));
            // Read customer's response
            //   printf("read buff:%s",read_buffer);
            rb = read(connection_fd, read_buffer, sizeof(read_buffer));
            read_buffer[strcspn(read_buffer, "\n")] = '\0';
            perror("reading");
            if (rb == -1)
            {
                perror("Error while reading from client");
                return false;
            }

            // Remove any newline characters from the input
            // read_buffer[strcspn(read_buffer, "\n")] = 0;
            // printf("read buff:%s",read_buffer);
            // fflush(stdout);
            // Parse the choice

            choice = atoi(read_buffer);
            switch (choice)
            {
            case 1:
                // Example operation for case 1 (e.g., view account details)
                //  sprintf(write_buffer, "You selected option 1: View Account Details\n");
                // printf("inside case 1:");
                // view_account_details(connection_fd,loggedInCustomer.ID);
                // perror("case1");

                get_balance(connection_fd);
                break;
            case 2:
                // Example operation for case 2 (e.g., transfer funds)
                // sprintf(write_buffer, "You selected option 2: Transfer Funds\n");
                deposit_money(connection_fd);
                break;

            case 3:
                withdraw_money(connection_fd);
                break;
            // Add more cases here as per your menu options
            case 4:
                transfer_funds(connection_fd);
                break;
            case 5:
                apply_for_loan(connection_fd);
                break;
            case 6:

                break;
            case 7:
                adding_feedback(connection_fd);
                break;
            case 8:
                get_transaction_details(connection_fd,loggedInCustomer.account_no);
                break;
            case 9:
                // Option to log out
                printf("Closing connection for customer.\n");
                wb = write(connection_fd, CUSTOMER_LOGOUT, strlen(CUSTOMER_LOGOUT));
                if (wb == -1)
                {
                    perror("Error while sending logout message to client");
                }
                return false; // Exit the function and return false
                break;
            default:
                // Invalid choice handling
                sprintf(write_buffer, "Invalid choice. Please try again.\n");
                break;
            }

            // Send response to client
            // wb = write(connection_fd, write_buffer, strlen(write_buffer));
            // if (wb == -1) {
            //     perror("Error writing response to client!");
            //     return false;
            // }

            // // Dummy read to sync with client
            // rb = read(connection_fd, read_buffer, sizeof(read_buffer));
        }
    }
    else
    {
        // If login fails, return false
        return false;
    }

    // Return true after successful operations
    return true;
}

bool get_balance(int connection_fd)
{
    char buff[2000], write_buffer[2000], read_buffer[2000];
    struct Account account;
    account.account_number = loggedInCustomer.account_no;

    // printf("loggedInCustomer.account_no-->: %d",loggedInCustomer.account_no);
    if (get_account_details(connection_fd, &account, true))
    {
        bzero(buff, sizeof(buff));
        // perror("getting balance---");
        //  printf("ACTIVE?%d",account.is_active);

        if (account.is_active)
        {
            // perror("inside  get balance details");
            // read(connection_fd, buff, sizeof(buff)); // Dummy read
            //  sprintf(buff, "You have ₹ %ld in your bank account", account.balance);
            //   strcat(buff, "\nRedirecting you to the main menu ...@");
            //  int wb=write(connection_fd, buff, strlen(buff));
            //  if (wb == -1)
            //  {
            //      perror("Error sending customer loginID and password to the client!");
            //      return false;
            //  }
            //  return true;
            // int rb = read(connection_fd, read_buffer, sizeof(read)); // Dummy read

            // bzero(write_buffer, sizeof(write_buffer));
            // sprintf(write_buffer, "%s%d", ADMIN_ADD_ACCOUNT_NUMBER, curr_account.account_number);

            // wb = write(connection_fd, write_buffer, sizeof(write_buffer));
        }
        else
            write(connection_fd, ACCOUNT_DEACTIVATED, strlen(ACCOUNT_DEACTIVATED));
        // return false;
        // read(connection_fd, buff, sizeof(buff)); // Dummy read
    }
    else
    {
        // ERROR while getting balance
        return false;
    }
    // return true;
}

bool deposit_money(int connection_fd)
{
    char read_buff[1000], write_buff[1000];
    ssize_t rb, wb;

    struct Account account;
    account.account_number = loggedInCustomer.account_no;

    long int deposit_amount = 0;

    //      Lock the critical section
    // struct sembuf semOp;
    // lock_critical_section(&semOp);

    if (get_account_details(connection_fd, &account, true))
    {
        // perror("IN@@");
        if (account.is_active)
        {
            wb = write(connection_fd, DEPOSIT_AMOUNT, strlen(DEPOSIT_AMOUNT));
            if (wb == -1)
            {
                perror("Error writing DEPOSIT_AMOUNT to client!");
                //    unlock_critical_section(&semOp);
                return false;
            }

            bzero(read_buff, sizeof(read_buff));
            rb = read(connection_fd, read_buff, sizeof(read_buff));
            read_buff[strcspn(read_buff, "\n")] = '\0';
            if (rb == -1)
            {
                perror("Error reading deposit money from client!");
                //  unlock_critical_section(&semOp);
                return false;
            }

            deposit_amount = atol(read_buff);
            if (deposit_amount != 0)
            {
                int newTransactionID = write_transaction_to_file(account.account_number, account.balance, account.balance + deposit_amount, 1);
                write_transaction_to_array(account.transactions, newTransactionID);

                account.balance += deposit_amount;

                int fd = open(ACCOUNT_FILE, O_RDWR);
                if (fd == -1)
                {
                    perror("Error opening account file!");
                    //   unlock_critical_section(&semOp);
                    return false;
                }

                struct Account tempAccount;
                bool account_found = false;
                while (read(fd, &tempAccount, sizeof(struct Account)) > 0)
                {
                    if (tempAccount.account_number == account.account_number)
                    {
                        account_found = true;
                        break;
                    }
                }

                if (account_found)
                {
                    off_t offset = lseek(fd, -sizeof(struct Account), SEEK_CUR);

                    if (offset == -1)
                    {
                        perror("Error seeking in account file!");
                        // unlock_critical_section(&semOp);
                        return false;
                    }

                    struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Account), getpid()};
                    int status = fcntl(fd, F_SETLKW, &lock);
                    if (status == -1)
                    {
                        perror("Error obtaining write lock on account file!");
                        // unlock_critical_section(&semOp);
                        return false;
                    }

                    wb = write(fd, &account, sizeof(struct Account));
                    if (wb == -1)
                    {
                        perror("Error storing updated deposit money in account record!");
                        // unlock_critical_section(&semOp);
                        return false;
                    }

                    lock.l_type = F_UNLCK;
                    fcntl(fd, F_SETLK, &lock);

                    write(connection_fd, DEPOSIT_AMOUNT_SUCCESS, strlen(DEPOSIT_AMOUNT_SUCCESS));
                    read(connection_fd, read_buff, sizeof(read_buff)); // Dummy read
                    read_buff[strcspn(read_buff, "\n")] = '\0';

                    get_balance(connection_fd);
                }
                else
                {
                    perror("Account not found!");
                    write(connection_fd, ACCOUNT_NOT_FOUND, strlen(ACCOUNT_NOT_FOUND));
                    read(connection_fd, read_buff, sizeof(read_buff)); // Dummy read
                }

                close(fd);
                //  unlock_critical_section(&semOp);
                return account_found;
            }
            else
            {
                wb = write(connection_fd, DEPOSIT_AMOUNT_INVALID, strlen(DEPOSIT_AMOUNT_INVALID));
                read(connection_fd, read_buff, sizeof(read_buff)); // Dummy read
            }
        }
        else
        {
            write(connection_fd, ACCOUNT_DEACTIVATED, strlen(ACCOUNT_DEACTIVATED));
            read(connection_fd, read_buff, sizeof(read_buff)); // Dummy read
        }

        //   unlock_critical_section(&semOp);
    }
    else
    {
        // FAIL
        //   unlock_critical_section(&semOp);
        return false;
    }
}

bool withdraw_money(int connection_fd)
{
    char read_buff[1000], write_buff[1000];
    ssize_t rb, wb;

    struct Account account;
    account.account_number = loggedInCustomer.account_no;

    long int withdraw_amount = 0;

    // Lock the critical section
    struct sembuf semOp;
    lock_critical_section(&semOp);

    if (get_account_details(connection_fd, &account, true))
    {
        if (account.is_active)
        {
            wb = write(connection_fd, WITHDRAW_AMOUNT, strlen(WITHDRAW_AMOUNT));
            if (wb == -1)
            {
                perror("Error writing WITHDRAW_AMOUNT message to client!");
                unlock_critical_section(&semOp);
                return false;
            }

            bzero(read_buff, sizeof(read_buff));
            rb = read(connection_fd, read_buff, sizeof(read_buff));
            read_buff[strcspn(read_buff, "\n")] = '\0';
            if (rb == -1)
            {
                perror("Error reading withdraw amount from client!");
                unlock_critical_section(&semOp);
                return false;
            }

            withdraw_amount = atol(read_buff);

            if (withdraw_amount != 0 && account.balance - withdraw_amount >= 0)
            {
                int newTransactionID= write_transaction_to_file(account.account_number, account.balance, account.balance - withdraw_amount, 0);
                write_transaction_to_array(account.transactions, newTransactionID);

                account.balance -= withdraw_amount;

                int fd = open(ACCOUNT_FILE, O_RDWR);
                if (fd == -1)
                {
                    perror("Error opening account file!");
                    unlock_critical_section(&semOp);
                    return false;
                }

                struct Account tempAccount;
                bool account_found = false;
                while (read(fd, &tempAccount, sizeof(struct Account)) > 0)
                {
                    if (tempAccount.account_number == account.account_number)
                    {
                        account_found = true;
                        break;
                    }
                }

                if (account_found)
                {
                    off_t offset = lseek(fd, -sizeof(struct Account), SEEK_CUR);

                    if (offset == -1)
                    {
                        perror("Error seeking in account file!");
                        unlock_critical_section(&semOp);
                        return false;
                    }

                    struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Account), getpid()};
                    int status = fcntl(fd, F_SETLKW, &lock);
                    if (status == -1)
                    {
                        perror("Error obtaining write lock on account record!");
                        unlock_critical_section(&semOp);
                        return false;
                    }

                    wb = write(fd, &account, sizeof(struct Account));
                    if (wb == -1)
                    {
                        perror("Error writing updated balance into account file!");
                        unlock_critical_section(&semOp);
                        return false;
                    }

                    lock.l_type = F_UNLCK;
                    fcntl(fd, F_SETLK, &lock);

                    write(connection_fd, WITHDRAW_AMOUNT_SUCCESS, strlen(WITHDRAW_AMOUNT_SUCCESS));
                    read(connection_fd, read_buff, sizeof(read_buff)); // Dummy read

                    get_balance(connection_fd);

                    close(fd);
                    unlock_critical_section(&semOp);
                    return true;
                }
                else
                {
                    perror("Account not found!");
                    write(connection_fd, ACCOUNT_NOT_FOUND, strlen(ACCOUNT_NOT_FOUND));
                    rb = read(connection_fd, read_buff, sizeof(read_buff));
                }

                close(fd);
                unlock_critical_section(&semOp);
                return account_found;
            }
            else
            {
                wb = write(connection_fd, WITHDRAW_AMOUNT_INVALID, strlen(WITHDRAW_AMOUNT_INVALID));
                rb = read(connection_fd, read_buff, sizeof(read_buff));
            }
        }
        else
        {
            write(connection_fd, ACCOUNT_DEACTIVATED, strlen(ACCOUNT_DEACTIVATED));
            rb = read(connection_fd, read_buff, sizeof(read_buff));
        }

        // read(connection_fd, read_buff, sizeof(read_buff)); // Dummy read
        unlock_critical_section(&semOp);
    }
    else
    {
        // FAILURE while getting account information
        unlock_critical_section(&semOp);
        return false;
    }
}

bool lock_critical_section(struct sembuf *semOp)
{
    semOp->sem_flg = SEM_UNDO;
    semOp->sem_op = -1;
    semOp->sem_num = 0;
    int semopStatus = semop(sem_id, semOp, 1);
    if (semopStatus == -1)
    {
        perror("Error while locking critical section");
        return false;
    }
    return true;
}

bool unlock_critical_section(struct sembuf *semOp)
{
    semOp->sem_op = 1;
    int semopStatus = semop(sem_id, semOp, 1);
    if (semopStatus == -1)
    {
        perror("Error while operating on semaphore!");
        _exit(1);
    }
    return true;
}

bool transfer_funds(int connection_fd)
{
    char read_buff[1000], write_buff[1000];
    ssize_t rb, wb;

    struct Account sender_account, recipient_account;
    sender_account.account_number = loggedInCustomer.account_no;

    long int transfer_amount = 0;
    long int recipient_account_number = 0;

    // Lock critical section
    struct sembuf semOp;
    lock_critical_section(&semOp);

    if (get_account_details(connection_fd, &sender_account, true))
    {
        if (sender_account.is_active)
        {
            wb = write(connection_fd, ENTER_RECIPIENT_ACCOUNT, strlen(ENTER_RECIPIENT_ACCOUNT));
            if (wb == -1)
            {
                perror("Error requesting recipient account from client!");
                unlock_critical_section(&semOp);
                return false;
            }

            bzero(read_buff, sizeof(read_buff));
            rb = read(connection_fd, read_buff, sizeof(read_buff));
            if (rb == -1)
            {
                perror("Error reading recipient account number from client!");
                unlock_critical_section(&semOp);
                return false;
            }

            recipient_account_number = atol(read_buff);

            // Fetch recipient account details
            if (!get_account_by_number(connection_fd, recipient_account_number, &recipient_account) || !recipient_account.is_active)
            {
                write(connection_fd, ACCOUNT_NOT_FOUND, strlen(ACCOUNT_NOT_FOUND));
                read(connection_fd, read_buff, sizeof(read_buff)); // Dummy read
                unlock_critical_section(&semOp);
                return false;
            }

            wb = write(connection_fd, ENTER_TRANSFER_AMOUNT, strlen(ENTER_TRANSFER_AMOUNT));
            if (wb == -1)
            {
                perror("Error requesting transfer amount!");
                unlock_critical_section(&semOp);
                return false;
            }

            bzero(read_buff, sizeof(read_buff));
            rb = read(connection_fd, read_buff, sizeof(read_buff));
            if (rb == -1)
            {
                perror("Error reading transfer amount from client!");
                unlock_critical_section(&semOp);
                return false;
            }

            transfer_amount = atol(read_buff);

            if (transfer_amount > 0 && sender_account.balance - transfer_amount >= 0)
            {
                int fd = open(ACCOUNT_FILE, O_RDWR);
                if (fd == -1)
                {
                    perror("Error opening account file");
                    unlock_critical_section(&semOp);
                    return false;
                }

                // Lock sender account, then recipient account to avoid deadlocks
                if (sender_account.account_number < recipient_account.account_number)
                {
                    lock_account(fd, sender_account.account_number);
                    lock_account(fd, recipient_account.account_number);
                }
                else
                {
                    lock_account(fd, recipient_account.account_number);
                    lock_account(fd, sender_account.account_number);
                }

                // Deduct from sender and credit to recipient
                sender_account.balance -= transfer_amount;
                recipient_account.balance += transfer_amount;
                printf("sender amoount = %ld", sender_account.balance);

                // Update both accounts
                if (!find_and_update_account_in_file(fd, sender_account.account_number, &sender_account, sender_account.balance) ||
                    !find_and_update_account_in_file(fd, recipient_account.account_number, &recipient_account, recipient_account.balance))
                {
                    perror("Error updating accounts in file");
                    unlock_account(fd, sender_account.account_number);
                    unlock_account(fd, recipient_account.account_number);
                    close(fd);
                    unlock_critical_section(&semOp);
                    return false;
                }

                // Unlock both accounts
                unlock_account(fd, sender_account.account_number);
                unlock_account(fd, recipient_account.account_number);

                write(connection_fd, TRANSFER_SUCCESS, strlen(TRANSFER_SUCCESS));
                read(connection_fd, read_buff, sizeof(read_buff)); // Dummy read
                close(fd);
                unlock_critical_section(&semOp);

                return true;
            }
            else
            {
                write(connection_fd, TRANSFER_INVALID, strlen(TRANSFER_INVALID));
                read(connection_fd, read_buff, sizeof(read_buff)); // Dummy read
            }
        }
        else
        {
            write(connection_fd, ACCOUNT_DEACTIVATED, strlen(ACCOUNT_DEACTIVATED));
            read(connection_fd, read_buff, sizeof(read_buff)); // Dummy read
        }

        unlock_critical_section(&semOp);
    }
    else
    {
        // Failure in getting sender account details
        unlock_critical_section(&semOp);
        return false;
    }
    return false;
}

bool adding_feedback(int connection_fd)
{
    char read_buffer[1000], write_buffer[1000];
    ssize_t rb, wb;
    struct Feedback curr_feedback, prev_feedback;
    // int customer_id = loggedInCustomer.ID;
    int fd = open(FEEDBACK_FILE, O_RDONLY);

    if (fd == -1 && errno == ENOENT)
    {
        // feedback file was never created
        curr_feedback.feedback_id = 0;
    }
    else if (fd == -1)
    {
        perror("Error while opening feedback records file");
        return false;
    }
    else
    {
        int offset = lseek(fd, -sizeof(struct Feedback), SEEK_END);
        if (offset == -1)
        {
            perror("Error seeking to last Feedback record!");
            return false;
        }
        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Feedback), getpid()};
        int status = fcntl(fd, F_SETLKW, &lock);

        if (status == -1)
        {
            perror("Error obtaining read lock on Feedback record!");
            return false;
        }
        rb = read(fd, &prev_feedback, sizeof(struct Feedback));
        if (rb == -1)
        {
            perror("Error while reading Feedback record from file!");
            return false;
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);

        curr_feedback.feedback_id = prev_feedback.feedback_id + 1;
    }
    bzero(write_buffer, sizeof(write_buffer));

    sprintf(write_buffer, "%s", ADD_FEEDBACK);

    wb = write(connection_fd, write_buffer, strlen(write_buffer)); // change
    if (wb == -1)
    {
        perror("Error writing ADD_FEEDBACK message to client!");
        return false;
    }

    rb = read(connection_fd, read_buffer, sizeof(read_buffer));
    if (rb == -1)
    {
        perror("Error reading ADD_FEEDBACK response from client!");
        return false;
    }
    // read_buffer[rb] = '\0';
    // printf("readbuff : %s", read_buffer);
    strcpy(curr_feedback.feedback_text, read_buffer);
    curr_feedback.customer_id = loggedInCustomer.ID;
    fd = open(FEEDBACK_FILE, O_CREAT | O_APPEND | O_WRONLY, 0777);
    if (fd == -1)
    {
        perror("Error while creating / opening feedback file!");
        return false;
    }
    wb = write(fd, &curr_feedback, sizeof(curr_feedback));
    if (wb == -1)
    {
        perror("Error while writing feedback record to file!");
        return false;
    }
    write(connection_fd, FEEDBACK_SUCCESS, strlen(FEEDBACK_SUCCESS));
    read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read
    close(fd);

    //    curr_feedback.feedback_text ="";
    return true;
}

bool apply_for_loan(int connection_fd)
{
    struct Loan new_loan;
    ssize_t rb, wb;
    char buffer[1000], write_buffer[1000], read_buffer[1000];

    // Generate a new loan ID (e.g., by reading the latest loan ID from file)
    new_loan.loan_id = generate_new_loan_id();
    new_loan.customer_id = loggedInCustomer.ID;

    // Get loan amount and duration from customer

    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));
    sprintf(write_buffer, "%s", "Enter loan amount :");
    wb = write(connection_fd, write_buffer, strlen(write_buffer)); // change
    if (wb == -1)
    {
        perror("Error writing Enter loan amount message to client!");
        return false;
    }

    rb = read(connection_fd, read_buffer, sizeof(read_buffer));
    if (rb == -1)
    {
        perror("Error reading Enter loan amount response from client!");
        return false;
    }
    read_buffer[rb] = '\0';
    // printf("readbuff : %s", read_buffer);

    new_loan.amount = atoi(read_buffer);
    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));

    sprintf(write_buffer, "%s", "Enter loan duration (in months) :");
    wb = write(connection_fd, write_buffer, strlen(write_buffer)); // change
    if (wb == -1)
    {
        perror("Error writing Enter loan duration (in months) message to client!");
        return false;
    }

    rb = read(connection_fd, read_buffer, sizeof(read_buffer));
    if (rb == -1)
    {
        perror("Error Enter loan duration (in months) from client!");
        return false;
    }
    read_buffer[rb] = '\0';
    // printf("readbuff : %s", read_buffer);
    // strcpy(new_loan.duration, read_buffer);
    new_loan.duration = atoi(read_buffer);

    // wb = write(connection_fd, "Enter loan amount: ", strlen("Enter loan amount: "));
    // rb = read(connection_fd, buffer, sizeof(buffer));
    // sscanf(buffer, "%lf", &new_loan.amount);

    // Set default status to "Pending"
    strcpy(new_loan.status, "Pending");

    // Set an interest rate (this can be a fixed rate or variable based on amount/duration)
    new_loan.interest_rate = assign_interest_rate(new_loan.amount, new_loan.duration);

    // Write loan application to the file
    int fd = open(LOAN_FILE, O_WRONLY | O_APPEND);
    if (fd == -1)
    {
        perror("Error while opening loan requests file");
        return false;
    }
    write(fd, &new_loan, sizeof(struct Loan));
    close(fd);

    // Notify the customer that the application has been submitted
    wb = write(connection_fd, "Loan application submitted successfully.^", strlen("Loan application submitted successfully.^"));
    rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read
    return true;
}

int generate_new_loan_id()
{
    int loan_id = 1000; // Start loan ID from 1000
    struct Loan loan;
    int fd = open(LOAN_FILE, O_RDONLY);
    if (fd != -1)
    {
        while (read(fd, &loan, sizeof(struct Loan)) > 0)
        {
            loan_id = loan.loan_id;
        }
        close(fd);
    }
    return loan_id + 1; // Return the next available loan ID
}

double assign_interest_rate(double amount, int duration)
{
    if (amount <= 100000)
    {
        return 5.0; // 5% interest for smaller amounts
    }
    else if (amount <= 500000)
    {
        return 7.0; // 7% interest for mid-range loans
    }
    else
    {
        return 9.0; // 9% interest for larger amounts
    }
}

void lock_account(int fd, long int account_number)
{
    // Calculate the offset for the account record in the file
    off_t offset = account_number * sizeof(struct Account);

    // Create a write lock for this section
    struct flock lock;
    lock.l_type = F_WRLCK; // Write lock
    lock.l_whence = SEEK_SET;
    lock.l_start = offset;               // Starting point
    lock.l_len = sizeof(struct Account); // Length of lock (size of account struct)
    lock.l_pid = getpid();

    // Apply the lock
    if (fcntl(fd, F_SETLKW, &lock) == -1)
    {
        perror("Error locking account record");
    }
}

void unlock_account(int fd, long int account_number)
{
    // Calculate the offset for the account record in the file
    off_t offset = account_number * sizeof(struct Account);

    // Create an unlock operation for this section
    struct flock lock;
    lock.l_type = F_UNLCK; // Unlock
    lock.l_whence = SEEK_SET;
    lock.l_start = offset;               // Starting point
    lock.l_len = sizeof(struct Account); // Length of unlock (size of account struct)
    lock.l_pid = getpid();

    // Release the lock
    if (fcntl(fd, F_SETLK, &lock) == -1)
    {
        perror("Error unlocking account record");
    }
}

bool find_and_update_account_in_file(int fd, long int account_number, struct Account *account, int balance_)
{
    struct Account temp_account;
    off_t offset;
    ssize_t rb, wb;

    // Reset file pointer to the beginning
    lseek(fd, 0, SEEK_SET);

    while ((rb = read(fd, &temp_account, sizeof(struct Account))) > 0)
    {
        if (temp_account.account_number == account_number)
        {
            // Update the account balance and other fields as needed
            temp_account.balance = balance_;
            // Seek back to the beginning of the found account record
            offset = lseek(fd, -sizeof(struct Account), SEEK_CUR);
            if (offset == -1)
            {
                perror("Error while seeking to account position");
                return false;
            }

            // Write updated data back
            wb = write(fd, &temp_account, sizeof(struct Account));
            if (wb == -1)
            {
                perror("Error while writing updated account info into file");
                return false;
            }
            return true; // Successful update
        }
    }

    if (rb == -1)
    {
        perror("Error while reading from file");
    }
    return false; // Account not found or error
}

int write_transaction_to_file(int accountNumber, long int oldBalance, long int newBalance, bool operation)
{
    struct Transaction cur_transaction;
    cur_transaction.account_number = accountNumber;
    cur_transaction.old_balance = oldBalance;
    cur_transaction.new_balance = newBalance;
    cur_transaction.operation = operation;
    cur_transaction.transaction_time = time(NULL);

    ssize_t rb, wb;

    int fd = open(TRANSACTION_FILE, O_CREAT | O_APPEND | O_RDWR, S_IRWXU);

    // Get most recent transaction number
    off_t offset = lseek(fd, -sizeof(struct Transaction), SEEK_END);
    if (offset >= 0)
    {
        // There exists at least one transaction record
        struct Transaction prev_transaction;
        rb = read(fd, &prev_transaction, sizeof(struct Transaction));

        cur_transaction.transaction_ID= prev_transaction.transaction_ID + 1;
    }
    else
        // No transaction records exist
        cur_transaction.transaction_ID = 0;

    wb = write(fd, &cur_transaction, sizeof(struct Transaction));

  close(fd);
     return cur_transaction.transaction_ID;
}

bool get_transaction_details(int connection_fd, int account_num) {
    ssize_t rb, wb;
    char read_buff[1000], write_buff[10000], buff[1000];
    struct Account account;

    // If account number is not provided, request it from the client
    // if (account_num == -1) {
    //     wb = write(connection_fd, GET_ACCOUNT_NUMBER, strlen(GET_ACCOUNT_NUMBER));
    //     if (wb == -1) {
    //         perror("Error writing GET_ACCOUNT_NUMBER message to client!");
    //         return false;
    //     }

    //     bzero(read_buff, sizeof(read_buff));
    //     rb = read(connection_fd, read_buff, sizeof(read_buff));
    //     if (rb == -1) {
    //         perror("Error reading account number response from client!");
    //         return false;
    //     }

    //     account.account_number = atoi(read_buff);
    // } else {
        account.account_number = account_num;
    // }

    // Retrieve account details
    if (get_account_details(connection_fd, &account, true)) {
        struct Transaction transaction;
        struct tm *transaction_time;
        int fd = open(TRANSACTION_FILE, O_RDONLY);
        if (fd == -1) {
            perror("Error while opening transaction file!");
            write(connection_fd, TRANSACTIONS_NOT_FOUND, strlen(TRANSACTIONS_NOT_FOUND));
            read(connection_fd, read_buff, sizeof(read_buff)); // Dummy read
            return false;
        }

        bzero(write_buff, sizeof(write_buff));
        bzero(read_buff, sizeof(read_buff));
        // Iterate through the transaction IDs
        for (int iter = 0; iter < MAX_TRANSACTIONS && account.transactions[iter] != -1; iter++) {
            int offset = lseek(fd, account.transactions[iter] * sizeof(struct Transaction), SEEK_SET);
            if (offset == -1) {
                perror("Error while seeking to required transaction record!");
                close(fd);
                return false;
            }

            struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Transaction), getpid()};
            if (fcntl(fd, F_SETLKW, &lock) == -1) {
                perror("Error obtaining read lock on transaction record!");
                close(fd);
                return false;
            }

            rb = read(fd, &transaction, sizeof(struct Transaction));
            if (rb == -1) {
                perror("Error reading transaction record from file!");
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
                close(fd);
                return false;
            }

            lock.l_type = F_UNLCK;
            fcntl(fd, F_SETLK, &lock);

            transaction_time = localtime(&(transaction.transaction_time));

            bzero(buff, sizeof(buff));
            snprintf(buff, sizeof(buff),
                "Details of transaction %d:\n"
                "\tDate: %02d:%02d %02d/%02d/%04d\n"
                "\tOperation: %s\n"
                "\tBalance:\n"
                "\t\tBefore: %ld\n"
                "\t\tAfter: %ld\n"
                "\t\tDifference: %ld\n",
                (iter + 1),
                transaction_time->tm_hour, transaction_time->tm_min,
                transaction_time->tm_mday, transaction_time->tm_mon + 1, transaction_time->tm_year + 1900,
                (transaction.operation ? "Deposit" : "Withdraw"),
                transaction.old_balance, transaction.new_balance,
                (transaction.new_balance - transaction.old_balance));

            // Safely concatenate to the write buffer
            if (strlen(write_buff) + strlen(buff) < sizeof(write_buff)) {
                strcat(write_buff, buff);
            } else {
                fprintf(stderr, "Warning: Write buffer overflow prevented.\n");
                break;
            }
        }

        close(fd);

        // Check if any transactions were found
        if (strlen(write_buff) == 0) {
            write(connection_fd, TRANSACTIONS_NOT_FOUND, strlen(TRANSACTIONS_NOT_FOUND));
            read(connection_fd, read_buff, sizeof(read_buff)); // Dummy read
            return false;
        } else {
            strcat(write_buff, "^");
            wb = write(connection_fd, write_buff, strlen(write_buff));
            if (wb == -1) {
                perror("Error writing transaction details to client!");
                return false;
            }
            read(connection_fd, read_buff, sizeof(read_buff)); // Dummy read to sync
        }
    }

    return true;
}



void write_transaction_to_array(int *transactionArray, int ID)
{
    // Check if there's any free space in the array to write the new transaction ID
    int iter = 0;
    while (transactionArray[iter] != -1)
        iter++;

    if (iter >= MAX_TRANSACTIONS)
    {
        // No space
        for (iter = 1; iter < MAX_TRANSACTIONS; iter++)
            // Shift elements one step back discarding the oldest transaction
            transactionArray[iter - 1] = transactionArray[iter];
        transactionArray[iter - 1] = ID;
    }
    else
    {
        // Space available
        transactionArray[iter] = ID;
    }
}


bool change_password(int connection_fd) {
    ssize_t rb, wb;
    char read_buff[1000], newPassword[1000], confirmPassword[1000];
    char hashedOldPassword[HASH_HEX_SIZE + 1], hashedNewPassword[HASH_HEX_SIZE + 1];
    
    // Lock the critical section
    struct sembuf semOp = {0, -1, SEM_UNDO};
    int semopStatus = semop(sem_id, &semOp, 1);
    if (semopStatus == -1) {
        perror("Error while locking critical section");
        return false;
    }

    // Step 1: Request old password
    wb = write(connection_fd, PASSWORD_CHANGE_OLD_PASS, strlen(PASSWORD_CHANGE_OLD_PASS));
    if (wb == -1) {
        perror("Error writing PASSWORD_CHANGE_OLD_PASS message to client!");
        unlock_critical_section(&semOp);
        return false;
    }

    bzero(read_buff, sizeof(read_buff));
    rb = read(connection_fd, read_buff, sizeof(read_buff));
    if (rb == -1) {
        perror("Error reading old password response from client");
        unlock_critical_section(&semOp);
        return false;
    }

    // Hash the entered old password
    unsigned char current_hash_pass[EVP_MAX_MD_SIZE];
    hash_password(read_buff, current_hash_pass);

    // Convert the hashed password to hexadecimal string
    char current_hex_hash[HASH_HEX_SIZE + 1];
    for (int i = 0; i < EVP_MD_size(EVP_sha256()); i++) {
        sprintf(&current_hex_hash[i * 2], "%02x", current_hash_pass[i]);
    }
    current_hex_hash[HASH_HEX_SIZE] = '\0'; // Null-terminate the string

    // Step 2: Check if the old password matches the stored password
    if (strcmp(current_hex_hash, loggedInCustomer.password) == 0) {
        // Password matches, proceed to request new password
        wb = write(connection_fd, PASSWORD_CHANGE_NEW_PASS, strlen(PASSWORD_CHANGE_NEW_PASS));
        if (wb == -1) {
            perror("Error writing PASSWORD_CHANGE_NEW_PASS message to client!");
            unlock_critical_section(&semOp);
            return false;
        }

        bzero(newPassword, sizeof(newPassword));
        rb = read(connection_fd, newPassword, sizeof(newPassword));
        if (rb == -1) {
            perror("Error reading new password response from client");
            unlock_critical_section(&semOp);
            return false;
        }

        // // Step 3: Request confirmation of new password
        // wb = write(connection_fd, PASSWORD_CHANGE_CONFIRM_PASS, strlen(PASSWORD_CHANGE_CONFIRM_PASS));
        // if (wb == -1) {
        //     perror("Error writing PASSWORD_CHANGE_CONFIRM_PASS message to client!");
        //     unlock_critical_section(&semOp);
        //     return false;
        // }

        // bzero(confirmPassword, sizeof(confirmPassword));
        // rb = read(connection_fd, confirmPassword, sizeof(confirmPassword));
        // if (rb == -1) {
        //     perror("Error reading confirmation password response from client");
        //     unlock_critical_section(&semOp);
        //     return false;
        // }

        // Step 4: Verify if new password matches the confirmation
        // if (strcmp(newPassword, confirmPassword) != 0) {
        //     wb = write(connection_fd, PASSWORD_CHANGE_NEW_PASS_INVALID, strlen(PASSWORD_CHANGE_NEW_PASS_INVALID));
        //     read(connection_fd, read_buff, sizeof(read_buff)); // Dummy read
        //     unlock_critical_section(&semOp);
        //     return false;
        // }

        // Step 5: Hash the new password
        unsigned char new_hash_pass[EVP_MAX_MD_SIZE];
        hash_password(newPassword, new_hash_pass);

        // Convert the hashed password to hexadecimal string
        for (int i = 0; i < EVP_MD_size(EVP_sha256()); i++) {
            sprintf(&hashedNewPassword[i * 2], "%02x", new_hash_pass[i]);
        }
        hashedNewPassword[HASH_HEX_SIZE] = '\0';

        // Update the password in the customer structure
        strcpy(loggedInCustomer.password, hashedNewPassword);

        // Step 6: Update password in the customer file
        int customerFileDescriptor = open(CUSTOMER_FILE, O_RDWR);
        if (customerFileDescriptor == -1) {
            perror("Error opening customer file!");
            unlock_critical_section(&semOp);
            return false;
        }

        // Locate the specific customer record using loggedInCustomer.ID
        off_t offset = lseek(customerFileDescriptor, loggedInCustomer.ID * sizeof(struct Customer), SEEK_SET);
        if (offset == -1) {
            perror("Error seeking to the customer record!");
            close(customerFileDescriptor);
            unlock_critical_section(&semOp);
            return false;
        }

        // Lock the file before updating
        struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};
        int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
        if (lockingStatus == -1) {
            perror("Error obtaining write lock on customer record!");
            close(customerFileDescriptor);
            unlock_critical_section(&semOp);
            return false;
        }

        // Write updated customer data back to the file
        wb = write(customerFileDescriptor, &loggedInCustomer, sizeof(struct Customer));
        if (wb == -1) {
            perror("Error storing updated customer password into customer record!");
            close(customerFileDescriptor);
            unlock_critical_section(&semOp);
            return false;
        }

        // Unlock the file after writing
        lock.l_type = F_UNLCK;
        fcntl(customerFileDescriptor, F_SETLK, &lock);
        close(customerFileDescriptor);

        // Notify client of success
        wb = write(connection_fd, PASSWORD_CHANGE_SUCCESS, strlen(PASSWORD_CHANGE_SUCCESS));
        rb = read(connection_fd, read_buff, sizeof(read_buff)); // Dummy read for response synchronization

        unlock_critical_section(&semOp);
        return true;
    } else {
        // Old password doesn't match
        wb = write(connection_fd, PASSWORD_CHANGE_OLD_PASS_INVALID, strlen(PASSWORD_CHANGE_OLD_PASS_INVALID));
        read(connection_fd, read_buff, sizeof(read_buff)); // Dummy read
    }

    unlock_critical_section(&semOp);
    return false;
}

   


// bool change_password(int connection_fd)
// {
//     ssize_t rb, wb;
//     char read_buffer[1000], write_buffer[1000], hashedPassword[1000];

//     char newPassword[1000];

//     // Lock the critical section
//     struct sembuf semOp = {0, -1, SEM_UNDO};
//     int semopStatus = semop(sem_id, &semOp, 1);
//     if (semopStatus == -1)
//     {
//         perror("Error while locking critical section");
//         return false;
//     }

//     wb = write(connFD, PASSWORD_CHANGE_OLD_PASS, strlen(PASSWORD_CHANGE_OLD_PASS));
//     if (wb == -1)
//     {
//         perror("Error writing PASSWORD_CHANGE_OLD_PASS message to client!");
//         unlock_critical_section(&semOp);
//         return false;
//     }

//     bzero(read_buffer, sizeof(read_buffer));
//     rb = read(connFD, read_buffer, sizeof(read_buffer));
//     if (rb == -1)
//     {
//         perror("Error reading old password response from client");
//         unlock_critical_section(&semOp);
//         return false;
//     }

//     if (strcmp(crypt(read_buffer, SALT_BAE), loggedInCustomer.password) == 0)
//     {
//         // Password matches with old password
//         wb = write(connFD, PASSWORD_CHANGE_NEW_PASS, strlen(PASSWORD_CHANGE_NEW_PASS));
//         if (wb == -1)
//         {
//             perror("Error writing PASSWORD_CHANGE_NEW_PASS message to client!");
//             unlock_critical_section(&semOp);
//             return false;
//         }
//         bzero(read_buffer, sizeof(read_buffer));
//         rb = read(connFD, read_buffer, sizeof(read_buffer));
//         if (rb == -1)
//         {
//             perror("Error reading new password response from client");
//             unlock_critical_section(&semOp);
//             return false;
//         }

//         strcpy(newPassword, crypt(read_buffer, SALT_BAE));

//         wb = write(connFD, PASSWORD_CHANGE_NEW_PASS_RE, strlen(PASSWORD_CHANGE_NEW_PASS_RE));
//         if (wb == -1)
//         {
//             perror("Error writing PASSWORD_CHANGE_NEW_PASS_RE message to client!");
//             unlock_critical_section(&semOp);
//             return false;
//         }
//         bzero(read_buffer, sizeof(read_buffer));
//         rb = read(connFD, read_buffer, sizeof(read_buffer));
//         if (rb == -1)
//         {
//             perror("Error reading new password reenter response from client");
//             unlock_critical_section(&semOp);
//             return false;
//         }

//         if (strcmp(crypt(read_buffer, SALT_BAE), newPassword) == 0)
//         {
//             // New & reentered passwords match

//             strcpy(loggedInCustomer.password, newPassword);

//             int customerFileDescriptor = open(CUSTOMER_FILE, O_WRONLY);
//             if (customerFileDescriptor == -1)
//             {
//                 perror("Error opening customer file!");
//                 unlock_critical_section(&semOp);
//                 return false;
//             }

//             off_t offset = lseek(customerFileDescriptor, loggedInCustomer.id * sizeof(struct Customer), SEEK_SET);
//             if (offset == -1)
//             {
//                 perror("Error seeking to the customer record!");
//                 unlock_critical_section(&semOp);
//                 return false;
//             }

//             struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};
//             int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
//             if (lockingStatus == -1)
//             {
//                 perror("Error obtaining write lock on customer record!");
//                 unlock_critical_section(&semOp);
//                 return false;
//             }

//             wb = write(customerFileDescriptor, &loggedInCustomer, sizeof(struct Customer));
//             if (wb == -1)
//             {
//                 perror("Error storing updated customer password into customer record!");
//                 unlock_critical_section(&semOp);
//                 return false;
//             }

//             lock.l_type = F_UNLCK;
//             lockingStatus = fcntl(customerFileDescriptor, F_SETLK, &lock);

//             close(customerFileDescriptor);

//             wb = write(connFD, PASSWORD_CHANGE_SUCCESS, strlen(PASSWORD_CHANGE_SUCCESS));
//             rb = read(connFD, read_buffer, sizeof(read_buffer)); // Dummy read

//             unlock_critical_section(&semOp);

//             return true;
//         }
//         else
//         {
//             // New & reentered passwords don't match
//             wb = write(connFD, PASSWORD_CHANGE_NEW_PASS_INVALID, strlen(PASSWORD_CHANGE_NEW_PASS_INVALID));
//             rb = read(connFD, read_buffer, sizeof(read_buffer)); // Dummy read
//         }
//     }
//     else
//     {
//         // Password doesn't match with old password
//         wb = write(connFD, PASSWORD_CHANGE_OLD_PASS_INVALID, strlen(PASSWORD_CHANGE_OLD_PASS_INVALID));
//         rb = read(connFD, read_buffer, sizeof(read_buffer)); // Dummy read
//     }

//     unlock_critical_section(&semOp);

//     return false;
// }
#endif