#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H

#include <stdio.h> // Import for `printf` & `perror` functions
#include <errno.h> // Import for `errno` variable

#include <fcntl.h>      // Import for `fcntl` functions
#include <unistd.h>     // Import for `fork`, `fcntl`, `read`, `write`, `lseek, `_exit` functions
#include <sys/types.h>  // Import for `socket`, `bind`, `listen`, `accept`, `fork`, `lseek` functions
#include <sys/socket.h> // Import for `socket`, `bind`, `listen`, `accept` functions
#include <netinet/ip.h> // Import for `sockaddr_in` stucture

#include <string.h>  // Import for string functions
#include <stdbool.h> // Import for `bool` data type
#include <stdlib.h>  // Import for `atoi` function

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




bool get_account_details(int connection_fd, struct Account *customer_account) {
    ssize_t rb, wb;
    char read_buffer[2000], write_buffer[2000];
    char buff[2000];

    struct Account account;
     perror("INSIDE.");
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
        return false;
    } else {
      //  printf("INSIDE ELSE++++++++++++++");

        perror("inside  get account  details");
        bzero(write_buffer, sizeof(write_buffer));
        sprintf(write_buffer, "Account Details - \n\tAccount Number : %d\n\tAccount Status : %s",
        account.account_number, (account.is_active) ? "Active" : "Deactivated");
        if (account.is_active) {
            sprintf(buff, "\n\tAccount Balance: ₹ %ld ", account.balance);
            strcat(write_buffer, buff);
            strcat(write_buffer, "\n@");
        }

        // strcat(write_buffer, "");
        wb = write(connection_fd, write_buffer, strlen(write_buffer));
        //  rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read
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


bool lock_account(int fd, long int account_num)
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


bool unlock_account(int fd, long int account_num)
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


#endif