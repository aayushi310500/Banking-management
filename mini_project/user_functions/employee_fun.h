#ifndef EMPLOYEE_FUN_H
#define EMPLOYEE_FUN_H

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
#include "../common_functions/admin_fun.h"

bool approve_reject_loans(int connection_fd, int employee_id)
{
    // int acc_id;
    char write_buffer[2000], read_buffer[2000];
    ssize_t wb, rb;
    struct Loan loan;

    int fd = open(LOAN_FILE, O_RDWR); // Open feedback file for reading

    if (fd == -1)
    {
        perror("Error opening feedback file");
        return false;
    }

    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));

    lseek(fd, 0, SEEK_SET);
    while (read(fd, &loan, sizeof(struct Loan)) > 0)
    {
        bzero(write_buffer, sizeof(write_buffer));
        bzero(read_buffer, sizeof(read_buffer));

        if (loan.employee_id == employee_id && loan.status == "Pending")
        {
            // wb = write(connection_fd, write_buffer, strlen(write_buffer));
            // if (wb == -1)
            // {
            //     perror("Error writing feedback message to client!");
            //     close(fd);
            //     return false;
            // }

            // read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read

            // bzero(write_buffer, sizeof(write_buffer));
            // bzero(read_buffer, sizeof(read_buffer));

            sprintf(write_buffer, "%s", APPROVE_REJECT);
            wb = write(connection_fd, write_buffer, strlen(write_buffer)); // change
            if (wb == -1)
            {
                perror("Error writing APPROVE_REJECT message to client!");
                return false;
            }

            rb = read(connection_fd, read_buffer, sizeof(read_buffer));
            if (rb == -1)
            {
                perror("Error reading APPROVE_REJECT from client!");
                return false;
            }
            read_buffer[rb] = '\0';

            int status = atoi(read_buffer);
            if (status == 1)
            {
                strcpy(loan.status, "Approved");
            }
            if (status == 0)
            {
                strcpy(loan.status, "Reject");
            }

            if (write(fd, &loan, sizeof(struct Loan)) == -1)
            {
                perror("Error updating user status");
                close(fd);
                continue;
            }

            bzero(write_buffer, sizeof(write_buffer));
            bzero(read_buffer, sizeof(read_buffer));

            sprintf(write_buffer, "%s", "loan approved/reject status updated correctly.^");
            wb = write(connection_fd, write_buffer, strlen(write_buffer)); // change
            if (wb == -1)
            {
                perror("Error writing APPROVE_REJECT message to client!");
                return false;
            }

            rb = read(connection_fd, read_buffer, sizeof(read_buffer));
        }
        lseek(fd, 0, SEEK_SET);

        bzero(write_buffer, sizeof(write_buffer));
        bzero(read_buffer, sizeof(read_buffer));

        sprintf(write_buffer, "%s", "redirecting to the main menu ^");
        wb = write(connection_fd, write_buffer, strlen(write_buffer)); // change
        if (wb == -1)
        {
            perror("Error writing APPROVE_REJECT message to client!");
            return false;
        }

        rb = read(connection_fd, read_buffer, sizeof(read_buffer));

        // At least one feedback found
        // if (feedback.is_reviewed == false)
        // {
        //     feedback.is_reviewed = true;
        //     feedback_found = true;

        //     if (write(fd, &feedback, sizeof(struct Feedback)) == -1)
        //     {
        //         perror("Error updating loan status");
        //         close(fd);
        //         return false;
        //     }
        //     snprintf(write_buffer, sizeof(write_buffer), "\nFeedback ID: %d\nCustomer ID: %d\nFeedback Text: %s",
        //              feedback.feedback_id, feedback.customer_id, feedback.feedback_text);
        //     strcat(write_buffer, "^");

        //     // Send the feedback to the manager over the socket
        //     wb = write(connection_fd, write_buffer, strlen(write_buffer));
        //     if (wb == -1)
        //     {
        //         perror("Error writing feedback message to client!");
        //         close(fd);
        //         return false;
        //     }
        //     int rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read
        // }

        // Prepare the feedback for sending
    }
}

bool view_assigned_loan_application(int connection_fd, int employee_id)
{
    char write_buffer[2000], read_buffer[2000];
    ssize_t wb, rb;
    struct Loan loan;

    int fd = open(LOAN_FILE, O_RDWR); // Open feedback file for reading

    if (fd == -1)
    {
        perror("Error opening feedback file");
        return false;
    }

    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));

   wb = write(connection_fd, "\n List of loans you are assigned : ^", strlen(write_buffer));
    if (wb == -1)
    {
        perror("Error writing feedback message to client!");
        close(fd); 
        return false;
    }
    read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read


    while (read(fd, &loan, sizeof(struct Loan)) > 0)
    {
        bzero(write_buffer, sizeof(write_buffer));
        bzero(read_buffer, sizeof(read_buffer));

        if (loan.employee_id == employee_id)
        {

            snprintf(write_buffer, sizeof(write_buffer), "\n\nLoan ID: %d\nCustomer ID: %d\nAmount: %f \nDuration: %d \nInterest Rate: %f \nStatus: %s \n Empoyee id assigned to: %d",
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
        }
    }

    wb = write(connection_fd, "\n Redirecting to main menu ^", strlen(write_buffer));
    if (wb == -1)
    {
        perror("Error writing feedback message to client!");
        close(fd);
        return false;
    }
    read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read

    lseek(fd, 0, SEEK_SET);
}

#endif