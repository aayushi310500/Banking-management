#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H

#include <stdio.h> 
#include <errno.h> 

#include <fcntl.h>     
#include <unistd.h>     
#include <sys/types.h>  
#include <sys/socket.h> 
#include <netinet/ip.h> 

#include <string.h>  
#include <stdbool.h> 
#include <stdlib.h>  

#include "../header_files/data.h"
#include "../struct_files/account.h"





// bool view_account_details(int connection_fd, int search_account_num, struct Account *customer_account) {
//     ssize_t rb, wb;
//     char read_buffer[2000], write_buffer[2000];
//     struct Account account;

//     int fd = open(ACCOUNT_FILE, O_RDONLY);
//     if (fd == -1) {
//         perror("Error while opening account file");
//         return false;
//     }

//     bool account_found = false;

//     while (true) {
//         struct flock lock = {F_RDLCK, SEEK_CUR, 0, sizeof(struct Account), getpid()};
        
//         int lockingStatus = fcntl(fd, F_SETLKW, &lock);
//         if (lockingStatus == -1) {
//             perror("Error obtaining read lock on account record!");
//             close(fd);
//             return false;
//         }

//         rb = read(fd, &account, sizeof(struct Account));
//         if (rb == -1) {
//             perror("Error reading account record from file!");
//             close(fd);
//             return false;
//         } else if (rb == 0) {
//             // End of file reached
//             break;
//         }

//         lock.l_type = F_UNLCK;
//         fcntl(fd, F_SETLK, &lock);

//         // Check if this is the account we're looking for
//         if (account.account_number == search_account_num) {
//             account_found = true;
//             if (customer_account != NULL) {
//                 *customer_account = account;
//             }
//             break;
//         }
//     }

//     close(fd);

//     if (!account_found) {
//         bzero(write_buffer, sizeof(write_buffer));
//         strcpy(write_buffer, ACCOUNT_ID_DOESNT_EXIST);
//         strcat(write_buffer, "^");
//         wb = write(connection_fd, write_buffer, strlen(write_buffer));
//         if (wb == -1) {
//             perror("Error writing ACCOUNT_ID_DOESNT_EXIST message to client!");
//             return false;
//         }
//         return false;
//     }

//     // If we are not just populating the customer_account struct, send details to client
//     if (customer_account == NULL) {
//         bzero(write_buffer, sizeof(write_buffer));
//         sprintf(write_buffer, "Account Details - \n\tAccount Number : %d\n\tAccount Status : %s", account.account_number, (account.is_active) ? "Active" : "Deactivated");
        
//         if (account.is_active) {
//             sprintf(write_buffer + strlen(write_buffer), "\n\tAccount Balance: ₹%ld", account.balance);
//         }
        
//         strcat(write_buffer, "\n^");
//         wb = write(connection_fd, write_buffer, strlen(write_buffer));
//         rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read
//     }

//     return true;
// }





bool get_account_details(int connection_fd, struct Account *customer_account,int flag) {
    ssize_t rb, wb;
    char read_buffer[2000], write_buffer[2000];
    char buff[2000];

    struct Account account;
    // perror("INSIDE.");
    int account_num;
    
    if (customer_account == NULL) {
        wb = write(connection_fd, GET_ACCOUNT_NUMBER, strlen(GET_ACCOUNT_NUMBER));
        if (wb == -1) {
            perror("Error writing GET_ACCOUNT_NUMBER message to client!");
            // close(fd); // Added to close the file before returning
            return false;
        }

        bzero(read_buffer, sizeof(read_buffer));
        rb = read(connection_fd, read_buffer, sizeof(read_buffer));
         read_buffer[strcspn(read_buffer, "\n")] = '\0';
        if (rb == -1) {
            perror("Error reading account number response from client!");
           // close(fd); // Added to close the file before returning
            return false;
        }

        account_num = atoi(read_buffer);
    } else {
        account_num = customer_account->account_number;
    }

 int fd = open(ACCOUNT_FILE, O_RDONLY);
    if (fd == -1)
    {
        // Account record doesn't exist
        bzero(write_buffer, sizeof(write_buffer));
        strcpy(write_buffer, ACCOUNT_ID_DOESNT_EXIST);
        strcat(write_buffer, "^");
        perror("Error opening account file in get_account_details!");
        wb = write(connection_fd, write_buffer, strlen(write_buffer));
        if (wb == -1)
        {
            perror("Error while writing ACCOUNT_ID_DOESNT_EXIT message to client!");
            return false;
        }
        rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read
        
        return false;
    }


    bool account_found = false;
    lseek(fd, 0, SEEK_SET);
    
    while (true) {
        struct flock lock = {F_RDLCK, SEEK_CUR, 0, sizeof(struct Account), getpid()};
        
        int lockingStatus = fcntl(fd, F_SETLKW, &lock);
        if (lockingStatus == -1) {
            perror("Error obtaining read lock on account record!");
            close(fd);
            return false;
        }

        rb = read(fd, &account, sizeof(struct Account));
        if (rb == -1) {
            perror("Error reading account record from file!");
            close(fd);
            return false;
        } else if (rb == 0) {
            // End of file reached
            break;
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        // Check if this is the account we're looking for
        if (account.account_number == account_num) {
            account_found = true;
            if (customer_account != NULL) {
                *customer_account = account;
            }
            break;
        }
    }

    close(fd);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      

    if (!account_found) {
        bzero(write_buffer, sizeof(write_buffer));
        strcpy(write_buffer, ACCOUNT_ID_DOESNT_EXIST);
        strcat(write_buffer, "^");
        wb = write(connection_fd, write_buffer, strlen(write_buffer));
        if (wb == -1) {
            perror("Error writing ACCOUNT_ID_DOESNT_EXIST message to client!");
            return false;
        }
        rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read
        return false;
    } else {                                                                                                                                                                                                                                                            
      //  printf("INSIDE ELSE++++++++++++++");
      if(flag){
        perror("inside  get account  details");
        bzero(write_buffer, sizeof(write_buffer));
        sprintf(write_buffer, "Account Details - \n\tAccount Number : %d\n\tAccount Status : %s",
        account.account_number, (account.is_active) ? "Active" : "Deactivated");
        if (account.is_active) {
            sprintf(buff, "\n\tAccount Balance: ₹ %ld", account.balance);
            strcat(write_buffer, buff);
            strcat(write_buffer, "^");
        }
        
        // strcat(write_buffer, "");
        wb = write(connection_fd, write_buffer, strlen(write_buffer));
       
      }
          rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read
    }
    return true;
}


bool get_account_by_number(int connection_fd, int account_num,struct Account *customer_account) {
    ssize_t rb, wb;
    char read_buffer[2000], write_buffer[2000];
    char buff[2000];

    struct Account account;
    // printf("INSIDE GET ACCOUNT DETAILS..");
    // int account_num;
    
    // if (customer_account == NULL) {
    //     wb = write(connection_fd, GET_ACCOUNT_NUMBER, strlen(GET_ACCOUNT_NUMBER));
    //     if (wb == -1) {
    //         perror("Error writing GET_ACCOUNT_NUMBER message to client!");
    //         // close(fd); // Added to close the file before returning
    //         return false;
    //     }

        // bzero(read_buffer, sizeof(read_buffer));
        // rb = read(connection_fd, read_buffer, sizeof(read_buffer));
        // if (rb == -1) {
        //     perror("Error reading account number response from client!");
        //    // close(fd); // Added to close the file before returning
        //     return false;
        // }

    //     account_num = atoi(read_buffer);
    // } else {
    //     account_num = customer_account->account_number;
    // }

 int fd = open(ACCOUNT_FILE, O_RDONLY);
    if (fd == -1)
    {
        // Account record doesn't exist
        bzero(write_buffer, sizeof(write_buffer));
        strcpy(write_buffer, ACCOUNT_ID_DOESNT_EXIST);
        strcat(write_buffer, "^");
        perror("Error opening account file in get_account_details!");
        wb = write(connection_fd, write_buffer, strlen(write_buffer));

        if (wb == -1)
        {
            perror("Error while writing ACCOUNT_ID_DOESNT_EXIT message to client!");
            return false;
        }
        rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read
        return false;
    }


    bool account_found = false;
    lseek(fd, 0, SEEK_SET);
    
    while (true) {
        struct flock lock = {F_RDLCK, SEEK_CUR, 0, sizeof(struct Account), getpid()};
        
        int lockingStatus = fcntl(fd, F_SETLKW, &lock);
        if (lockingStatus == -1) {
            perror("Error obtaining read lock on account record!");
            close(fd);
            return false;
        }

        rb = read(fd, &account, sizeof(struct Account));
        if (rb == -1) {
            perror("Error reading account record from file!");
            close(fd);
            return false;
        } else if (rb == 0) {
            // End of file reached
            break;
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        // Check if this is the account we're looking for
        if (account.account_number == account_num) {
            account_found = true;
            if (customer_account != NULL) {
                *customer_account = account;
            }
            break;
        }
    }

    close(fd);

    if (!account_found) {
        bzero(write_buffer, sizeof(write_buffer));
        strcpy(write_buffer, ACCOUNT_ID_DOESNT_EXIST);
        strcat(write_buffer, "^");
        wb = write(connection_fd, write_buffer, strlen(write_buffer));
        
        if (wb == -1) {
            perror("Error writing ACCOUNT_ID_DOESNT_EXIST message to client!");
            return false;
        }
        rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read   
        return false;
    } 
    //  else {
        //printf("INSIDE ELSE++++++++++++++");

        // perror("inside  get account  details");
        // bzero(write_buffer, sizeof(write_buffer));
        // sprintf(write_buffer, "Account Details - \n\tAccount Number : %d\n\tAccount Status : %s",
        // account.account_number, (account.is_active) ? "Active" : "Deactivated");
        // if (account.is_active) {
        //     sprintf(buff, "\n\tAccount Balance: ₹ %ld ", account.balance);
        //     strcat(write_buffer, buff);
        //      //strcat(write_buffer, "\n");
        // }

        // strcat(write_buffer, "\n^");
       // wb = write(connection_fd, write_buffer, strlen(write_buffer));
        //  rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read
    //  }
   
    return true;
}


bool update_account_in_file(int fd, struct Account *account)
{
    struct Account tempAccount;
    bool account_found = false;

    // Loop through the file to find the account
    lseek(fd, 0, SEEK_SET); // Move to the beginning of the file
    while (read(fd, &tempAccount, sizeof(struct Account)) > 0)
    {
        if (tempAccount.account_number == account->account_number)
        {
            // Seek back to the position of the found account
            lseek(fd, -sizeof(struct Account), SEEK_CUR);

            // Write the updated account information
            if (write(fd, account, sizeof(struct Account)) == -1)
            {
                perror("Error updating account in file");
                return false;
            }
            account_found = true;
            break;
        }
    }

    return account_found;
}


bool lock_account_(int fd, long int account_num)
{
    struct Account tempAccount;
    
    // Move to the start of the file
    lseek(fd, 0, SEEK_SET);
    
    while (read(fd, &tempAccount, sizeof(struct Account)) > 0)
    {
        if (tempAccount.account_number == account_num)
        {
            // Get the current position to apply the lock
            off_t offset = lseek(fd, -sizeof(struct Account), SEEK_CUR);
            if (offset == -1)
            {
                perror("Error seeking in account file");
                return false;
            }

            struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Account), getpid()};
            if (fcntl(fd, F_SETLKW, &lock) == -1)
            {
                perror("Error locking account record");
                return false;
            }
            return true;
        }
    }
    return false; // Account not found
}


bool unlock_account_(int fd, long int account_num)
{
    struct Account tempAccount;
    
    // Move to the start of the file
    lseek(fd, 0, SEEK_SET);
    
    while (read(fd, &tempAccount, sizeof(struct Account)) > 0)
    {
        if (tempAccount.account_number == account_num)
        {
            // Get the current position to apply the unlock
            off_t offset = lseek(fd, -sizeof(struct Account), SEEK_CUR);
            if (offset == -1)
            {
                perror("Error seeking in account file");
                return false;
            }

            struct flock lock = {F_UNLCK, SEEK_SET, offset, sizeof(struct Account), getpid()};
            if (fcntl(fd, F_SETLK, &lock) == -1)
            {
                perror("Error unlocking account record");
                return false;
            }
            return true;
        }
    }
    return false; // Account not found
}


bool get_transaction_details(int connection_fd, int account_num)
{
    ssize_t rb, wb;
    char read_buff[1000], write_buff[10000], buff[1000];
    struct Account account;

    // If account number is not provided, request it from the client
    if (account_num == -1) {
        wb = write(connection_fd, GET_ACCOUNT_NUMBER, strlen(GET_ACCOUNT_NUMBER));
        if (wb == -1) {
            perror("Error writing GET_ACCOUNT_NUMBER message to client!");
            return false;
        }

        bzero(read_buff, sizeof(read_buff));
        rb = read(connection_fd, read_buff, sizeof(read_buff));
        if (rb == -1) {
            perror("Error reading account number response from client!");
            return false;
        }

        account.account_number = atoi(read_buff);
    } else {
    account.account_number = account_num;
    }

    // Retrieve account details
    if (get_account_details(connection_fd, &account, true))
    {
        struct Transaction transaction;
        struct tm *transaction_time;
        int fd = open(TRANSACTION_FILE, O_RDONLY);
        if (fd == -1)
        {
            perror("Error while opening transaction file!");
            write(connection_fd, TRANSACTIONS_NOT_FOUND, strlen(TRANSACTIONS_NOT_FOUND));
            read(connection_fd, read_buff, sizeof(read_buff)); // Dummy read
            return false;
        }

        bzero(write_buff, sizeof(write_buff));
        bzero(read_buff, sizeof(read_buff));
        // Iterate through the transaction IDs
        for (int iter = 0; iter < MAX_TRANSACTIONS && account.transactions[iter] != -1; iter++)
        {
            int offset = lseek(fd, account.transactions[iter] * sizeof(struct Transaction), SEEK_SET);
            if (offset == -1)
            {
                perror("Error while seeking to required transaction record!");
                close(fd);
                return false;
            }

            struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Transaction), getpid()};
            if (fcntl(fd, F_SETLKW, &lock) == -1)
            {
                perror("Error obtaining read lock on transaction record!");
                close(fd);
                return false;
            }

            rb = read(fd, &transaction, sizeof(struct Transaction));
            if (rb == -1)
            {
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
            if (strlen(write_buff) + strlen(buff) < sizeof(write_buff))
            {
                strcat(write_buff, buff);
            }
            else
            {
                fprintf(stderr, "Warning: Write buffer overflow prevented.\n");
                break;
            }
        }

        close(fd);

        // Check if any transactions were found
        if (strlen(write_buff) == 0)
        {
            write(connection_fd, TRANSACTIONS_NOT_FOUND, strlen(TRANSACTIONS_NOT_FOUND));
            read(connection_fd, read_buff, sizeof(read_buff)); // Dummy read
            return false;
        }
        else
        {
            strcat(write_buff, "^");
            wb = write(connection_fd, write_buff, strlen(write_buff));
            if (wb == -1)
            {
                perror("Error writing transaction details to client!");
                return false;
            }
            read(connection_fd, read_buff, sizeof(read_buff)); // Dummy read to sync
        }
    }

    return true;
}

#endif