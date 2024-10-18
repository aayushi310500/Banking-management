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

bool get_balance(int connection_fd);
bool deposit_money(int connection_fd);
bool withdraw_money(int connection_fd);
bool lock_critical_section(struct sembuf *semOp);
bool unlock_critical_section(struct sembuf *semOp);
bool transfer_funds(int connection_fd);
bool adding_feedback(int connection_fd);

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

        // key_t semKey = ftok(CUSTOMER_FILE, loggedInCustomer.account_no); // Generate a key based on the account number hence, different customers will have different semaphores

        // union semun
        // {
        //     int val; // Value of the semaphore
        // } semSet;

        // int semctlStatus;
        // sem_id = semget(semKey, 1, 0); // Get the semaphore if it exists
        // if (sem_id == -1)
        // {
        //     sem_id = semget(semKey, 1, IPC_CREAT | 0700); // Create a new semaphore
        //     if (sem_id == -1)
        //     {
        //         perror("Error while creating semaphore!");
        //         _exit(1);
        //     }

        //     semSet.val = 1; // Set a binary semaphore
        //     semctlStatus = semctl(sem_id, 0, SETVAL, semSet);
        //     if (semctlStatus == -1)
        //     {
        //         perror("Error while initializing a binary sempahore!");
        //         _exit(1);
        //     }
        // }
        //  bzero(write_buffer, sizeof(write_buffer));
        while (1)
        {
            printf("IIIIINNNNNN");
            // Send customer menu
            bzero(write_buffer, sizeof(write_buffer));
            bzero(read_buffer, sizeof(read_buffer));
            printf("\nCustomer menu\n");
            wb = write(connection_fd, CUSTOMER_MENU, strlen(CUSTOMER_MENU));
            if (wb == -1)
            {
                perror("Error while sending menu to the customer");
                return false;
            }
            bzero(read_buffer, sizeof(read_buffer)); // Clear buffer

            // bzero(write_buffer, sizeof(write_buffer));
            // Read customer's response
            //   printf("read buff:%s",read_buffer);
            rb = read(connection_fd, read_buffer, sizeof(read_buffer));
            read_buffer[strcspn(read_buffer, "\n")] = '\0';
            // perror("reading");
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

                break;
            case 7:
                adding_feedback(connection_fd);
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
    bzero(buff, sizeof(buff));
    // printf("loggedInCustomer.account_no-->: %d",loggedInCustomer.account_no);
    if (get_account_details(connection_fd, &account ,true))
    {

        // printf("ACTIVE?%d",account.is_active);

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
            return true;
            // int rb = read(connection_fd, read_buffer, sizeof(read)); // Dummy read

            // bzero(write_buffer, sizeof(write_buffer));
            // sprintf(write_buffer, "%s%d", ADMIN_ADD_ACCOUNT_NUMBER, curr_account.account_number);

            // wb = write(connection_fd, write_buffer, sizeof(write_buffer));
        }
        else
            write(connection_fd, ACCOUNT_DEACTIVATED, strlen(ACCOUNT_DEACTIVATED));
        return false;
        // read(connection_fd, buff, sizeof(buff)); // Dummy read
    }
    else
    {
        // ERROR while getting balance
        return false;
    }
    return true;
}

bool deposit_money(int connection_fd)
{
    char read_buff[1000], write_buff[1000];
    ssize_t rb, wb;

    struct Account account;
    account.account_number = loggedInCustomer.account_no;

    long int deposit_amount = 0;

    // Lock the critical section
    // struct sembuf semOp;
    // lock_critical_section(&semOp);

    if (get_account_details(connection_fd, &account,false))
    {
        if (account.is_active)
        {
            wb = write(connection_fd, DEPOSIT_AMOUNT, strlen(DEPOSIT_AMOUNT));
            if (wb == -1)
            {
                perror("Error writing DEPOSIT_AMOUNT to client!");
        //        unlock_critical_section(&semOp);
                return false;
            }

            bzero(read_buff, sizeof(read_buff));
            rb = read(connection_fd, read_buff, sizeof(read_buff));
            read_buff[strcspn(read_buff, "\n")] = '\0';
            if (rb == -1)
            {
                perror("Error reading deposit money from client!");
   //             unlock_critical_section(&semOp);
                return false;
            }

            deposit_amount = atol(read_buff);
            if (deposit_amount != 0)
            {
                // int newTransactionID = write_transaction_to_file(account.accountNumber, account.balance, account.balance + depositAmount, 1);
                // write_transaction_to_array(account.transactions, newTransactionID);

                account.balance += deposit_amount;

                int fd = open(ACCOUNT_FILE, O_RDWR);
                if (fd == -1)
                {
                    perror("Error opening account file!");
       //             unlock_critical_section(&semOp);
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
                      //  unlock_critical_section(&semOp);
                        return false;
                    }

                    struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Account), getpid()};
                    int status = fcntl(fd, F_SETLKW, &lock);
                    if (status == -1)
                    {
                        perror("Error obtaining write lock on account file!");
                        //unlock_critical_section(&semOp);
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
                }

                close(fd);
               // unlock_critical_section(&semOp);
                return account_found;
            }
            else
            {
                wb = write(connection_fd, DEPOSIT_AMOUNT_INVALID, strlen(DEPOSIT_AMOUNT_INVALID));
            }
        }
        else
        {
            write(connection_fd, ACCOUNT_DEACTIVATED, strlen(ACCOUNT_DEACTIVATED));
        }
        read(connection_fd, read_buff, sizeof(read_buff)); // Dummy read

      //  unlock_critical_section(&semOp);
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

    if (get_account_details(connection_fd, &account,false))
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
                }

                close(fd);
                unlock_critical_section(&semOp);
                return account_found;
            }
            else
            {
                wb = write(connection_fd, WITHDRAW_AMOUNT_INVALID, strlen(WITHDRAW_AMOUNT_INVALID));
            }
        }
        else
        {
            write(connection_fd, ACCOUNT_DEACTIVATED, strlen(ACCOUNT_DEACTIVATED));
        }

        read(connection_fd, read_buff, sizeof(read_buff)); // Dummy read
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

    if (get_account_details(connection_fd, &sender_account,false))
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
                // Lock both accounts for update
                int fd = open(ACCOUNT_FILE, O_RDWR);
                lock_account(fd, sender_account.account_number);
                lock_account(fd, recipient_account.account_number);

                // Deduct from sender and credit to recipient
                sender_account.balance -= transfer_amount;
                recipient_account.balance += transfer_amount;

                // Update both accounts
                update_account_in_file(fd, &sender_account);
                update_account_in_file(fd, &recipient_account);

                // Unlock both accounts
                unlock_account(fd, sender_account.account_number);
                unlock_account(fd, recipient_account.account_number);

                write(connection_fd, TRANSFER_SUCCESS, strlen(TRANSFER_SUCCESS));
                close(fd);
                unlock_critical_section(&semOp);

                return true;
            }
            else
            {
                write(connection_fd, TRANSFER_INVALID, strlen(TRANSFER_INVALID));
            }
        }
        else
        {
            write(connection_fd, ACCOUNT_DEACTIVATED, strlen(ACCOUNT_DEACTIVATED));
        }

        read(connection_fd, read_buff, sizeof(read_buff)); // Dummy read
        unlock_critical_section(&semOp);
    }
    else
    {
        // Failure in getting sender account details
        unlock_critical_section(&semOp);
        return false;
    }
}

bool adding_feedback(int connection_fd)
{
    char read_buffer[1000], write_buffer[1000];
    ssize_t rb, wb;
    struct Feedback curr_feedback, prev_feedback;
    int customer_id = loggedInCustomer.ID;
    int fd = open(FEEDBACK_FILE, O_RDONLY);

    if (fd == -1 && errno == ENOENT)
    {
        // Customer file was never created
        curr_feedback.feedback_id = 0;
    }
    else if (fd == -1)
    {
        perror("Error while opening customer records file(admin_fun.h)");
        return false;
    }
    else
    {
        int offset = lseek(fd, -sizeof(struct Customer), SEEK_END);
        if (offset == -1)
        {
            perror("Error seeking to last Customer record!");
            return false;
        }
        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};
        int status = fcntl(fd, F_SETLKW, &lock);

        if (status == -1)
        {
            perror("Error obtaining read lock on Customer record!");
            return false;
        }
        rb = read(fd, &prev_feedback, sizeof(struct Customer));
        if (rb == -1)
        {
            perror("Error while reading Customer record from file!");
            return false;
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);

        curr_feedback.feedback_id = prev_feedback.feedback_id + 1;
    }

    sprintf(write_buffer, "%s", ADD_FEEDBACK);
    wb = write(connection_fd, write_buffer, strlen(write_buffer)); // change
    if (wb == -1)
    {
        perror("Error writing ADMIN_ADD_CUSTOMER_NAME message to client!");
        return false;
    }

    rb = read(connection_fd, read_buffer, sizeof(read_buffer));
    if (rb == -1)
    {
        perror("Error reading customer name response from client!");
        return false;
    }
    read_buffer[rb] = '\0';
    // printf("readbuff : %s", read_buffer);
    strcpy(curr_feedback.feedback_text, read_buffer);

    fd = open(FEEDBACK_FILE, O_CREAT | O_APPEND | O_WRONLY, 0777);
    if (fd == -1)
    {
        perror("Error while creating / opening customer file!");
        return false;
    }
    wb = write(fd, &curr_feedback, sizeof(curr_feedback));
    if (wb == -1)
    {
        perror("Error while writing Customer record to file!");
        return false;
    }
    close(fd);

    //    curr_feedback.feedback_text ="";
    return true;
}

// bool change_password(int connFD) {
//     ssize_t readBytes, writeBytes;
//     char readBuffer[1000], writeBuffer[1000], newPassword[1000], confirmPassword[1000];
//     char hashedOldPassword[HASH_HEX_SIZE + 1], hashedNewPassword[HASH_HEX_SIZE + 1];
//    // hash_password
//     // Lock the critical section
//     struct sembuf semOp = {0, -1, SEM_UNDO};
//     int semopStatus = semop(sem_id, &semOp, 1);
//     if (semopStatus == -1) {
//         perror("Error while locking critical section");
//         return false;
//     }

//     // Step 1: Request old password
//     writeBytes = write(connFD, PASSWORD_CHANGE_OLD_PASS, strlen(PASSWORD_CHANGE_OLD_PASS));
//     if (writeBytes == -1) {
//         perror("Error writing PASSWORD_CHANGE_OLD_PASS message to client!");
//         unlock_critical_section(&semOp);
//         return false;
//     }

//     bzero(readBuffer, sizeof(readBuffer));
//     readBytes = read(connFD, readBuffer, sizeof(readBuffer));
//     if (readBytes == -1) {
//         perror("Error reading old password response from client");
//         unlock_critical_section(&semOp);
//         return false;
//     }

//     // Hash the entered old password
//     // Parse the file content to extract the login ID and hashed password
//     char stored_login[MAX_LINE];
//     char stored_hash[HASH_HEX_SIZE + 1]; // Only store the first 64 hex characters
//    // sscanf(read_buffer_file, "Login ID: %s\nPassword (hashed): %64s\n", stored_login, stored_hash);
//     stored_hash[HASH_HEX_SIZE] = '\0'; // Ensure null termination of the hash

//     // Hash the input password using the same hash_password function
//     unsigned char current_hash_pass[EVP_MAX_MD_SIZE];
//     hash_password(readBuffer, current_hash_pass);

//     // Convert the hashed password to hexadecimal string
//     char current_hex_hash[HASH_HEX_SIZE + 1];
//     for (int i = 0; i < EVP_MD_size(EVP_sha256()); i++)
//     {
//         sprintf(&current_hex_hash[i * 2], "%02x", current_hash_pass[i]);
//     }
//     current_hex_hash[HASH_HEX_SIZE] = '\0'; // Null-terminate the string

//    // hash_password(readBuffer, hashedOldPassword);

//     // Step 2: Check if the old password matches
//     if (strcmp(current_hex_hash, loggedInCustomer.password) == 0) {
//         // Password matches, proceed to request new password
//         writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS, strlen(PASSWORD_CHANGE_NEW_PASS));
//         if (writeBytes == -1) {
//             perror("Error writing PASSWORD_CHANGE_NEW_PASS message to client!");
//             unlock_critical_section(&semOp);
//             return false;
//         }

//         bzero(newPassword, sizeof(newPassword));
//         readBytes = read(connFD, newPassword, sizeof(newPassword));
//         if (readBytes == -1) {
//             perror("Error reading new password response from client");
//             unlock_critical_section(&semOp);
//             return false;
//         }

//         // Step 3: Request re-enter of new password
//         writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS_RE, strlen(PASSWORD_CHANGE_NEW_PASS_RE));
//         if (writeBytes == -1) {
//             perror("Error writing PASSWORD_CHANGE_NEW_PASS_RE message to client!");
//             unlock_critical_section(&semOp);
//             return false;
//         }

//         bzero(confirmPassword, sizeof(confirmPassword));
//         readBytes = read(connFD, confirmPassword, sizeof(confirmPassword));
//         if (readBytes == -1) {
//             perror("Error reading new password reenter response from client");
//             unlock_critical_section(&semOp);
//             return false;
//         }

//         // Check if new password matches re-entered password
//         if (strcmp(newPassword, confirmPassword) == 0) {
//             // Hash the new password
//             // hash_password(newPassword, hashedNewPassword);

//           // Parse the file content to extract the login ID and hashed password
//     char stored_login[MAX_LINE];
//     char stored_hash[HASH_HEX_SIZE + 1]; // Only store the first 64 hex characters
//    // sscanf(read_buffer_file, "Login ID: %s\nPassword (hashed): %64s\n", stored_login, stored_hash);
//     stored_hash[HASH_HEX_SIZE] = '\0'; // Ensure null termination of the hash

//     // Hash the input password using the same hash_password function
//     unsigned char current_hash_pass[EVP_MAX_MD_SIZE];
//     hash_password(readBuffer, current_hash_pass);

//     // Convert the hashed password to hexadecimal string
//     char current_hex_hash[HASH_HEX_SIZE + 1];
//     for (int i = 0; i < EVP_MD_size(EVP_sha256()); i++)
//     {
//         sprintf(&current_hex_hash[i * 2], "%02x", current_hash_pass[i]);
//     }
//     current_hex_hash[HASH_HEX_SIZE] = '\0'; // Null-terminate the string

//    // hash_password(readBuffer, hashedOldPassword);

//             strcpy(loggedInCustomer.password, hashedNewPassword);

//             // Step 4: Update password in customer file
//             int customerFileDescriptor = open(CUSTOMER_FILE, O_RDWR);
//             if (customerFileDescriptor == -1) {
//                 perror("Error opening customer file!");
//                 unlock_critical_section(&semOp);
//                 return false;
//             }

//             // Locate the specific customer record using loggedInCustomer.id
//             off_t offset = lseek(customerFileDescriptor, loggedInCustomer.ID * sizeof(struct Customer), SEEK_SET);
//             if (offset == -1) {
//                 perror("Error seeking to the customer record!");
//                 close(customerFileDescriptor);
//                 unlock_critical_section(&semOp);
//                 return false;
//             }

//             // Lock the file before updating
//             struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};
//             int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
//             if (lockingStatus == -1) {
//                 perror("Error obtaining write lock on customer record!");
//                 close(customerFileDescriptor);
//                 unlock_critical_section(&semOp);
//                 return false;
//             }

//             // Write updated customer data back to the file
//             writeBytes = write(customerFileDescriptor, &loggedInCustomer, sizeof(struct Customer));
//             if (writeBytes == -1) {
//                 perror("Error storing updated customer password into customer record!");
//                 close(customerFileDescriptor);
//                 unlock_critical_section(&semOp);
//                 return false;
//             }

//             // Unlock the file after writing
//             lock.l_type = F_UNLCK;
//             fcntl(customerFileDescriptor, F_SETLK, &lock);

//             close(customerFileDescriptor);

//             // Notify client of success
//             writeBytes = write(connFD, PASSWORD_CHANGE_SUCCESS, strlen(PASSWORD_CHANGE_SUCCESS));
//             readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read for response synchronization

//             unlock_critical_section(&semOp);
//             return true;
//         } else {
//             // New passwords don't match
//             writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS_INVALID, strlen(PASSWORD_CHANGE_NEW_PASS_INVALID));
//             readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
//         }
//     } else {
//         // Old password doesn't match
//         writeBytes = write(connFD, PASSWORD_CHANGE_OLD_PASS_INVALID, strlen(PASSWORD_CHANGE_OLD_PASS_INVALID));
//         readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
//     }

//     unlock_critical_section(&semOp);
//     return false;
// }

// bool change_password(int connFD)
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