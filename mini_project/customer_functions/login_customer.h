#ifndef LOGIN_CUSTOMER
#define LOGIN_CUSTOMER

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
// #include "../header_files/admin_cred.h"
#include "../struct_files/user.h"
#include "../struct_files/customer.h"
// #include "admin_pass.h"
#include "../common_functions/hash_password.h"
// #include "../struct_files/admin.h"
// #include "authenticate_admin.h"

#define MAX_LINE 1024
#define HASH_HEX_SIZE 64

// int authenticate_customer(int connection_fd,const char *input_login, const char *input_pass) {

// //Open the customer file
// struct User customer;
//  char read_buffer_file[MAX_LINE];
//     char read_buffer[2000], write_buffer[2000];
//     ssize_t rb, wb;
//     int customerFileFD = open(USER_FILE, O_RDONLY);
//     if (customerFileFD == -1)
//     {
//         perror("Error opening customer file in read mode!");
//         return false;
//     }
//     bool userFound = false;
//     //Iterate through the file to find the matching login ID
//     while (read(customerFileFD, &customer, sizeof(struct Customer)) > 0)
//     {
//         if (strcmp(customer.login_id, input_login) == 0)
//         {
//             userFound = true;
//             break; // Stop searching once the customer is found
//         }
//     }
//      //read_buffer_file[rb] = '\0';
//    // Close the file after reading
//     close(customerFileFD);

//     char stored_login[MAX_LINE];
//     char stored_hash[HASH_HEX_SIZE + 1];  // Only store the first 64 hex characters
//   //  sscanf(read_buffer_file, "Login ID: %s\nPassword (hashed): %64s\n", stored_login, stored_hash);
//     stored_hash[HASH_HEX_SIZE] = '\0';  // Ensure null termination of the hash

//      if (strcmp(input_login, customer.login_id) != 0) {
//         // printf("Login ID does not match!\n");

//     sprintf(write_buffer, "%s", INVALID_LOGIN_MSG);
//     wb = write(connection_fd, write_buffer, strlen(write_buffer));
//     if (wb == -1)
//     {
//         perror("Error writing INVALID_LOGIN_MSG message to client!");
//         exit(1);
//     }

//     unsigned char hash_pass[EVP_MAX_MD_SIZE];  // Buffer for the hashed password
//     hash_password(input_pass, hash_pass);

//     // Convert the hashed password to hexadecimal string
//     char hex_hash[HASH_HEX_SIZE + 1];
//     for (int i = 0; i < EVP_MD_size(EVP_sha256()); i++) {  // Use the size for SHA-256
//         sprintf(&hex_hash[i * 2], "%02x", hash_pass[i]);
//     }
//     hex_hash[HASH_HEX_SIZE] = '\0';  // Null-ter

//        if (strcmp(hex_hash, customer.password) == 0) {

//          sprintf(write_buffer, "%s", LOGIN_SUCCESSFULL_MSG);
//         wb = write(connection_fd, write_buffer, strlen(write_buffer));
//     if (wb == -1)
//     {
//         perror("Error writing LOGIN_SUCCESSFULL_MSG message to client!");

//         exit(1);
//         return 0;
//     }
//     // rb = read(connection_fd, read_buffer, sizeof(read_buffer));//dummy read
//         //printf("Authentication successful!\n");
//         //return 1;  // Authentication success
//     } else {
//          sprintf(write_buffer, "%s", INVALID_PASSWORD_MSG);
//         wb = write(connection_fd, write_buffer, strlen(write_buffer));
//         if (wb == -1)
//     {
//         perror("Error writing INVALID_PASSWORD_MSG message to client!");
//         exit(1);
//     }
//         //printf("Password does not match!\n");
//         return 0;  // Authentication failed
//     }
//    // rb = read(connection_fd, read_buffer, sizeof(read_buffer));
//      return 1;
//     }

// }

// bool login_handler_customer(int connFD, struct Customer *ptrToCustomerID)
// {
//     ssize_t readBytes, writeBytes;            // Number of bytes written to / read from the socket
//     char readBuffer[1000], writeBuffer[1000]; // Buffer for reading from / writing to the client
//     struct Customer customer;

//     bzero(readBuffer, sizeof(readBuffer));
//     bzero(writeBuffer, sizeof(writeBuffer));

//     // Send login welcome message
//     strcpy(writeBuffer, CUSTOMER_LOGIN_WELCOME);
//     strcat(writeBuffer, "\n");
//     strcat(writeBuffer, LOGIN_ID);

//     writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
//     if (writeBytes == -1)
//     {
//         perror("Error writing WELCOME & LOGIN_ID message to the client!");
//         return false;
//     }

//     // Read login ID from client
//     readBytes = read(connFD, readBuffer, sizeof(readBuffer));
//     if (readBytes == -1)
//     {
//         perror("Error reading login ID from client!");
//         return false;
//     }

//     //bool userFound = false;
//     char login_id[30];
//     strcpy(login_id, readBuffer);

//     // Open the customer file
//     // int customerFileFD = open(CUSTOMER_FILE, O_RDONLY);
//     // if (customerFileFD == -1)
//     // {
//     //     perror("Error opening customer file in read mode!");
//     //     return false;
//     // }

//     // Iterate through the file to find the matching login ID
//     // while (read(customerFileFD, &customer, sizeof(struct Customer)) > 0)
//     // {
//     //     if (strcmp(customer.login_id, login_id) == 0)
//     //     {
//     //         userFound = true;
//     //         break; // Stop searching once the customer is found
//     //     }
//     // }

//     // Close the file after reading
//     // close(customerFileFD);

//     // if (userFound)
//     // {
//         // Ask for password
//         bzero(writeBuffer, sizeof(writeBuffer));
//         writeBytes = write(connFD, PASSWORD, strlen(PASSWORD));
//         if (writeBytes == -1)
//         {
//             perror("Error writing PASSWORD message to client!");
//             return false;
//         }

//         bzero(readBuffer, sizeof(readBuffer));
//         readBytes = read(connFD, readBuffer, sizeof(readBuffer));
//         if (readBytes == -1)
//         {
//             perror("Error reading password from the client!");
//             return false;
//         }

//         char password[40];
//             strcpy(password,readBuffer);

//         //Compare hashed passwords
//        if(authenticate_customer(connFD,login_id,password)){
//                 return true;
//             }
//     // }
//     // else
//     // {
//     //     // No matching login ID found
//     //     bzero(writeBuffer, sizeof(writeBuffer));
//     //     writeBytes = write(connFD, INVALID_LOGIN, strlen(INVALID_LOGIN));
//     // }

//     return false;
// }

// Function to authenticate customer based on login ID and password
bool authenticate_customer(int connection_fd, const char *input_login, const char *input_pass)
{
    struct Customer customer;
    char read_buffer[2000], write_buffer[2000];
    ssize_t wb;
    int customerFileFD = open(CUSTOMER_FILE, O_RDONLY);

    if (customerFileFD == -1)
    {
        perror("Error opening customer file in read mode!");
        return false;
    }

    bool userFound = false;
    // printf("customer_stored_login: %s",customer.login_id);
    //  printf("input_login: %s",input_login);
    // Iterate through the file to find the matching login ID
    lseek(customerFileFD, 0, SEEK_SET);
    ssize_t bytesRead;
    while ((bytesRead=read(customerFileFD, &customer, sizeof(struct Customer))) > 0)
    {
        // printf("customer_stored_login: %s\n", customer.login);
        // printf("input_login: %s\n", input_login);
        if (strcmp(customer.login, input_login) == 0)
        {
            userFound = true;
            break; // Stop searching once the customer is found
        }
    }
    if (bytesRead == -1) {
    perror("Error reading from customer file!");
}

    close(customerFileFD);

    // If user not found, send invalid login message
    if (!userFound)
    {
        sprintf(write_buffer, "%s", INVALID_LOGIN_MSG);
        wb = write(connection_fd, write_buffer, strlen(write_buffer));
        if (wb == -1)
        {
            perror("Error writing INVALID_LOGIN_MSG message to client!");
        }
        return false;
    }

    // Hash the input password
    unsigned char hash_pass[EVP_MAX_MD_SIZE];
    hash_password(input_pass, hash_pass);

    // Convert the hashed password to hexadecimal string
    char hex_hash[HASH_HEX_SIZE + 1];
    for (int i = 0; i < EVP_MD_size(EVP_sha256()); i++)
    {
        sprintf(&hex_hash[i * 2], "%02x", hash_pass[i]);
    }
    hex_hash[HASH_HEX_SIZE] = '\0'; // Ensure null termination

    // Compare the hashed input password with the stored password hash
    // printf("Stored Password Hash: %s\n", customer.password);
    // printf("Input Password Hash: %s\n", hex_hash);

    if (strcmp(hex_hash, customer.password) == 0)
    {
        // sprintf(write_buffer, "%s", LOGIN_SUCCESSFULL_MSG);
        // // printf("%s", write_buffer);
        // wb = write(connection_fd, write_buffer, strlen(write_buffer));
        // if (wb == -1)
        // {
        //     perror("Error writing LOGIN_SUCCESSFULL_MSG message to client!");
        //     return false;
        // }
       // int rb = read(connection_fd, read_buffer, sizeof(read_buffer));//dummy read
        return true; // Authentication success
    }
    else
    {
        sprintf(write_buffer, "%s", INVALID_PASSWORD_MSG);
        wb = write(connection_fd, write_buffer, strlen(write_buffer));
        if (wb == -1)
        {
            perror("Error writing INVALID_PASSWORD_MSG message to client!");
        }
        return false; // Authentication failed
    }
    return true;
}

// Function for handling customer login
bool login_handler_customer(int connFD, struct Customer *ptrToCustomerID)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[2000], writeBuffer[2000];

    bzero(readBuffer, sizeof(readBuffer));
    bzero(writeBuffer, sizeof(writeBuffer));

    // Send login welcome message
    strcpy(writeBuffer, CUSTOMER_LOGIN_WELCOME);
    strcat(writeBuffer, "\n");
    strcat(writeBuffer, LOGIN_ID);

    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing WELCOME & LOGIN_ID message to the client!");
        return false;
    }

    // Read login ID from client
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading login ID from client!");
        return false;
    }

    char login_id[30];
    strcpy(login_id, readBuffer);
    login_id[strcspn(login_id, "\n")] = '\0'; // Remove newline if present

    // Ask for password
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, PASSWORD);
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing PASSWORD message to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading password from the client!");
        return false;
    }

    char password[40];
    strcpy(password, readBuffer);
    password[strcspn(password, "\n")] = '\0'; // Remove newline if present

    // Authenticate the user
    if (authenticate_customer(connFD, login_id, password))
    {
       printf("return from login handler");
        return true;
    }

    return false;
}

#endif
