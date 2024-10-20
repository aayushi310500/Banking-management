#ifndef LOGIN_EMPLOYEE
#define LOGIN_EMPLOYEE

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


bool authenticate_user_(int connection_fd, const char *input_login, const char *input_pass, struct User *authenticate_user,int role_)
{
    struct User user;
    char write_buffer[2000];
    ssize_t wb;
    int fd = open(USER_FILE, O_RDONLY);

    if (fd == -1)   
    {
        perror("Error opening user file in read mode!");
        return false; // Return -1 for authentication error
    }

    bool userFound = false;
    lseek(fd, 0, SEEK_SET);
    ssize_t bytesRead;
    while ((bytesRead = read(fd, &user, sizeof(struct User))) > 0)
    {
        if (strcmp(user.login_id, input_login) == 0 && user.is_active)
        {
            userFound = true;
            break;
        }
    }
    if (bytesRead == -1) {
        perror("Error reading from user file!");
    }

    close(fd);

    if (!userFound || user.role != role_)
    {
        sprintf(write_buffer, "%s", INVALID_LOGIN_MSG);
        wb = write(connection_fd, write_buffer, strlen(write_buffer));
        if (wb == -1)
        {
            perror("Error writing INVALID_LOGIN_MSG message to client!");
        }
        return false;
    }
 
    unsigned char hash_pass[EVP_MAX_MD_SIZE];
    hash_password(input_pass, hash_pass);


    char hex_hash[HASH_HEX_SIZE + 1];
    for (int i = 0; i < EVP_MD_size(EVP_sha256()); i++)
    {
        sprintf(&hex_hash[i * 2], "%02x", hash_pass[i]);
    }
    hex_hash[HASH_HEX_SIZE] = '\0';
    
    perror("IN");
    printf("Input Password Hash: %s\n", hex_hash);
    printf("Stored Password Hash: %s\n", user.password);


    if (strcmp(hex_hash, user.password) == 0)
    {
        *authenticate_user = user; // Copy authenticated user data back
        return true; // Return 2 for Manager, 1 for Employee
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


bool login_handler_user(int connection_fd, struct User *authenticate_user,int role_)
{
    ssize_t rb, wb;
    char read_buff[2000], write_buff[2000];

    bzero(read_buff, sizeof(read_buff));
    bzero(write_buff, sizeof(write_buff));

    strcpy(write_buff, USER_LOGIN_WELCOME);
    strcat(write_buff, "\n");
    strcat(write_buff, LOGIN_ID);

    wb = write(connection_fd, write_buff, strlen(write_buff));
    if (wb == -1)
    {
        perror("Error writing WELCOME & LOGIN_ID message to the client!");
        return false;
    }

    rb = read(connection_fd, read_buff, sizeof(read_buff));
    if (rb == -1)
    {
        perror("Error reading login ID from client!");
        return false;
    }

    char login_id[30];
    strcpy(login_id, read_buff);
    login_id[strcspn(login_id, "\n")] = '\0';

    bzero(write_buff, sizeof(write_buff));
    strcpy(write_buff, PASSWORD);
    wb = write(connection_fd, write_buff, strlen(write_buff));
    if (wb == -1)
    {
        perror("Error writing PASSWORD message to client!");
        return false;
    }

    bzero(read_buff, sizeof(read_buff));
    rb = read(connection_fd, read_buff, sizeof(read_buff));
    if (rb == -1)
    {
        perror("Error reading password from the client!");
        return false;
    }

    char password[40];
    strcpy(password, read_buff);
    password[strcspn(password, "\n")] = '\0';

    if (authenticate_user_(connection_fd, login_id, password,authenticate_user,role_))
    {
       printf("return from login handler");
        return true;
    }

    return false;
}


#endif