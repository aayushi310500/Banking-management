#ifndef EMPLOYEE_FUN_H
#define EMPLOYEE_FUN_H

#include <stdio.h>     
#include <unistd.h>    
#include <string.h>    
#include <stdbool.h>   
#include <sys/types.h> 
#include <sys/stat.h>  
#include <fcntl.h>     
#include <stdlib.h>    
#include <errno.h>     

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
#include "../common_functions/admin_fun.h"

bool approve_reject_loans(int connection_fd, int employee_id)
{
    char write_buffer[2000], read_buffer[2000];
    ssize_t wb, rb;
    struct Loan loan;

    int fd = open(LOAN_FILE, O_RDWR); // Open loan file for reading and writing

    if (fd == -1)
    {
        perror("Error opening loan file");
        return false;
    }

    printf("Loan file opened successfully.\n"); // Debug
     fflush(stdout);
    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));

    lseek(fd, 0, SEEK_SET); // Start reading from the beginning of the file

    bool loans_found = false; // Track if there are any loans to process

    // Loop through each loan record
    while (read(fd, &loan, sizeof(struct Loan)) > 0)
    {
        printf("Read Loan ID: %d, Employee ID: %d, Status: %s\n", loan.loan_id, loan.employee_id, loan.status);
        fflush(stdout);
        printf("from file %d\n", loan.employee_id);
        fflush(stdout);
        printf("entered: %d\n", employee_id);
        fflush(stdout);
        if (loan.employee_id == employee_id && strcmp(loan.status, "Pending") == 0)
        {
            loans_found = true;

            // Inform the employee about the pending loan details
            snprintf(write_buffer, sizeof(write_buffer),
                     "Loan ID: %d\nCustomer ID: %d\nAmount: %.2f\nDuration: %d\nInterest Rate: %.2f\nStatus: %s\n^",
                     loan.loan_id, loan.customer_id, loan.amount, loan.duration, loan.interest_rate, loan.status);

            wb = write(connection_fd, write_buffer, strlen(write_buffer));
            if (wb == -1)
            {
                perror("Error writing loan details to client!");
                close(fd);
                return false;
            }

            // Sync with client
            rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read to sync
            if (rb == -1)
            {
                perror("Error reading sync response from client!");
                close(fd);
                return false;
            }

            // Ask for approval or rejection
            bzero(write_buffer, sizeof(write_buffer));
            sprintf(write_buffer, "Enter 1 to Approve, 0 to Reject: ^");
            wb = write(connection_fd, write_buffer, strlen(write_buffer));
            if (wb == -1)
            {
                perror("Error writing approval request to client!");
                close(fd);
                return false;
            }

            rb = read(connection_fd, read_buffer, sizeof(read_buffer));
            if (rb == -1)
            {
                perror("Error reading approval response from client!");
                close(fd);
                return false;
            }
            read_buffer[rb] = '\0';

            int status = atoi(read_buffer);
            if (status == 1)
            {
                strcpy(loan.status, "Approved");
            }
            else if (status == 0)
            {
                strcpy(loan.status, "Rejected");
            }
            else
            {
                // Invalid response, skip this loan
                continue;
            }

            // Seek back to the position of the loan record to update it
            lseek(fd, -sizeof(struct Loan), SEEK_CUR);
            if (write(fd, &loan, sizeof(struct Loan)) == -1)
            {
                perror("Error updating loan status");
                close(fd);
                return false;
            }

            // Confirm to the client that the status was updated
            bzero(write_buffer, sizeof(write_buffer));
            sprintf(write_buffer, "Loan status updated successfully.^");
            wb = write(connection_fd, write_buffer, strlen(write_buffer));
            if (wb == -1)
            {
                perror("Error writing loan status update confirmation!");
                close(fd);
                return false;
            }

            // Sync with client
            rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read to sync
            if (rb == -1)
            {
                perror("Error reading sync response from client!");
                close(fd);
                return false;
            }
        }
    }

    if (!loans_found)
    {
        // Inform the employee that there are no pending loans assigned
        sprintf(write_buffer, "No pending loans assigned to you.^");
        wb = write(connection_fd, write_buffer, strlen(write_buffer));
        if (wb == -1)
        {
            perror("Error writing no loans found message!");
            close(fd);
            return false;
        }

        read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read to sync
    }

    // Redirect to main menu
    bzero(write_buffer, sizeof(write_buffer));
    sprintf(write_buffer, "Redirecting to the main menu... ^");
    wb = write(connection_fd, write_buffer, strlen(write_buffer));
    if (wb == -1)
    {
        perror("Error writing redirect message!");
        close(fd);
        return false;
    }

    read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read to sync

    // Close the file
    close(fd);
    return true;
}

bool view_assigned_loan_application(int connection_fd, int employee_id)
{
    char write_buffer[2000], read_buffer[2000];
    ssize_t wb, rb;
    struct Loan loan;

    int fd = open(LOAN_FILE, O_RDWR); // Open the loan file for reading

    if (fd == -1)
    {
        perror("Error opening loan file");
        return false;
    }

    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));

    // Inform client that the list of loans will be displayed
    wb = write(connection_fd, "\nList of loans you are assigned: ^", strlen("\nList of loans you are assigned: ^"));
    if (wb == -1)
    {
        perror("Error writing to client!");
        close(fd);
        return false;
    }
    read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read to sync

    // Read each loan record and check if it matches the employee ID
    bool loans_found = false; // Track if any loans are found
    while (read(fd, &loan, sizeof(struct Loan)) > 0)
    {
        bzero(write_buffer, sizeof(write_buffer));
        bzero(read_buffer, sizeof(read_buffer));

        if (loan.employee_id == employee_id)
        {
            loans_found = true;
            snprintf(write_buffer, sizeof(write_buffer),
                     "\n\nLoan ID: %d\nCustomer ID: %d\nAmount: %.2f\nDuration: %d\nInterest Rate: %.2f\nStatus: %s\nEmployee ID assigned to: %d^",
                     loan.loan_id, loan.customer_id, loan.amount, loan.duration, loan.interest_rate, loan.status, loan.employee_id);
            strcat(write_buffer, "^");
            // Send the loan information to the client
            wb = write(connection_fd, write_buffer, strlen(write_buffer));
            if (wb == -1)
            {
                perror("Error writing loan details to client!");
                close(fd);
                return false;
            }
            read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read to sync
        }
    }

    // Inform the client if no loans were found
    if (!loans_found)
    {
        wb = write(connection_fd, "\nNo loans assigned to you.^", strlen("\nNo loans assigned to you.^"));
        if (wb == -1)
        {
            perror("Error writing message to client!");
            close(fd);
            return false;
        }
        read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read to sync
    }

    // Redirect to main menu
    wb = write(connection_fd, "\nRedirecting to main menu... ^", strlen("\nRedirecting to main menu... ^"));
    if (wb == -1)
    {
        perror("Error writing redirection message to client!");
        close(fd);
        return false;
    }
    read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read to sync

    // Close the file
    close(fd);
    return true;
}

#endif