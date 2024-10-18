#ifndef LOGIN_FUNCTIONS
#define LOGIN_FUNCTIONS

#include <stdio.h>     // Import for `printf` & `perror`
#include <unistd.h>    // Import for `read`, `write & `lseek`
#include <string.h>    // Import for string functions
#include <stdbool.h>   // Import for `bool` data type
#include <sys/types.h> // Import for `open`, `lseek`
#include <sys/stat.h>  // Import for `open`
#include <fcntl.h>     // Import for `open`
#include <stdlib.h>    // Import for `atoi`
#include <errno.h>     // Import for `errno`
#include <sodium.h>


#include <openssl/evp.h>
#include <openssl/sha.h>  


#include "../header_files/data.h"
#include "../header_files/admin_cred.h"
//  #include "../struct_files/customer.h"
#include "admin_pass.h"
#include "hash_password.h"
// #include "../struct_files/admin.h"
#include "authenticate_admin.h"


bool login_handler(bool isAdmin, int sock_fd, struct Customer *ptrToCustomerID)
{
    ssize_t rb, wb;                             // Number of bytes written to / read from the socket
    char read_buffer[1000], write_buffer[1000]; // Buffer for reading from / writing to the client
    char buff[1000];
    // struct Customer customer;

    // int ID;
    bzero(read_buffer, sizeof(read_buffer));
    bzero(write_buffer, sizeof(write_buffer));

    if (isAdmin)
        strcpy(write_buffer, ADMIN_LOGIN_WELCOME_MSG);
    else{}
       // strcpy(write_buffer, CUSTOMER_LOGIN_WELCOME_MSG);

    strcat(write_buffer, "\n");
    strcat(write_buffer, LOGIN_ID_MSG);

    wb = write(sock_fd, write_buffer, sizeof(write_buffer));
    if (wb == -1)
    {
        perror("Error writing WELCOME & LOGIN_ID message to the client!");
        return false;
    }
    rb = read(sock_fd, read_buffer, sizeof(read_buffer));
    if (rb == -1)
    {
        perror("Error reading login ID from client!");
        return false;
    }
     char login_id[30];
    strcpy(login_id,read_buffer);  
    // bool userFound = false;
    if (isAdmin)
    {     
        
        // if (strcmp(read_buffer, ADMIN_LOGIN_ID) == 0)
        //     userFound = true;
        // else
        // {
        // }
        // if (userFound)
        // {
            bzero(write_buffer, sizeof(write_buffer));
            wb = write(sock_fd, PASSWORD_MSG, strlen(PASSWORD_MSG));
            if (wb == -1)
            {
                perror("Error while writing PASSWORD message to client!");
                return false;
            }
            bzero(read_buffer, sizeof(read_buffer));
            rb = read(sock_fd, read_buffer, sizeof(read_buffer));
            if (rb == 1)
            {
                perror("Error reading password from the client!");
                return false;
            }
            char password[30];
            strcpy(password,read_buffer);

            //char hashed_password[1000];
            if(authenticate_admin(sock_fd,login_id,password)){
                return true;
            }
                    
                
        //     if (isAdmin)
        //     {
        //      if (crypto_pwhash_str_verify(hashed_password,ADMIN_PASSWORD , strlen(ADMIN_PASSWORD)) == 0) {
        //           return true;
        //      } else {
        //             //printf("Password verification failed.\n");
        //          }
        //             // strcpy(hashed_password, crypt(read_buffer, SALT));

        //     bzero(write_buffer, sizeof(write_buffer));
        //     wb = write(sock_fd, INVALID_PASSWORD_MSG, strlen(INVALID_PASSWORD_MSG));
        // }
        // else
        // {
        //     bzero(write_buffer, sizeof(write_buffer));
        //     wb = write(sock_fd, INVALID_LOGIN_MSG, strlen(INVALID_LOGIN_MSG));
        // }

        return false;
    // }
}
}

#endif