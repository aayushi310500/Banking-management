#ifndef USER_FUN_H
#define USER_FUN_H

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
#include "../struct_files/feedback.h"
#include "../common_functions/hash_password.h"
#include "../struct_files/loan.h"
#include "../user_functions/employee_fun.h"
#include "../user_functions/manager_fun.h"

int sem_id;
bool user_operation_handler(int connection_fd,int role_)
{
    // Assume `loggedInCustomer` is properly declared and initialized elsewhere
    struct User loggedInUser;

    // Attempt to log in the customer
    int ans = login_handler_user(connection_fd, &loggedInUser ,role_);
    if (ans && role_==2)
    {
        char read_buffer[1000], write_buffer[1000];
        ssize_t rb, wb;
        int choice;

        key_t semKey = ftok(USER_FILE, loggedInUser.ID); // Generate a key based on the account number hence, different customers will have different semaphores

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
        strcpy(write_buffer, MANAGER_LOGIN_SUCCESS);
        while (1)
        {
            strcat(write_buffer, "\n");
            strcat(write_buffer, MANAGER_MENU);
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
             //   perror("reading");
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

                activate_deactivate_user_(connection_fd);
                break;
            case 2:
                // Example operation for case 2 (e.g., transfer funds)
                // sprintf(write_buffer, "You selected option 2: Transfer Funds\n");
                //  deposit_money(connection_fd);
                assign_loan_to_employee(connection_fd);
                break;

            case 3:
               // withdraw_money(connection_fd);
               review_customer_feedbacks(connection_fd);
                break;
               // Add more cases here as per your menu options
            case 4:
               //  transfer_funds(connection_fd);
                break;
            case 5:
               view_account_detail(connection_fd);
               break;
            case 6:
                // Option to log out
                printf("Closing connection for customer.\n");
                wb = write(connection_fd, USER_LOGOUT, strlen(USER_LOGOUT));
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
    else if(ans && role_ ==1){
        char read_buffer[1000], write_buffer[1000];
        ssize_t rb, wb;
        int choice;

        key_t semKey = ftok(USER_FILE, loggedInUser.ID); // Generate a key based on the account number hence, different customers will have different semaphores

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
        strcpy(write_buffer, EMPLOYEE_LOGIN_SUCCESS);
        while (1)
        {
            strcat(write_buffer, "\n");
            strcat(write_buffer, EMPLOYEE_MENU);
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
             //   perror("reading");
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
               add_account(connection_fd);
                break;
            case 2:
                  modify_customer_details(connection_fd);
                break;
            case 3:
             
                break;
            case 4:
                 approve_reject_loans(connection_fd,loggedInUser.ID);
                break;
          
            case 5:
               get_transaction_details(connection_fd,-1);
            case 6:
               
            break;
            case 7:
                printf("Closing connection for customer.\n");
                wb = write(connection_fd, USER_LOGOUT, strlen(USER_LOGOUT));
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




#endif