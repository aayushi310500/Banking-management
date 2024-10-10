#ifndef ADMIN_FUNCTIONS
#define ADMIN_FUNCTIONS

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

// #include "login.h"
#include "../header_files/data.h"
#include "../struct_files/customer.h"
#include "hash_password.h"



void add_customers(int connection_fd)
{
    char read_buffer[1000], write_buffer[1000];
    ssize_t rb, wb;
    struct Customer curr_customer, prev_customer;

    int fd = open("/home/aayushi312000/Desktop/Documents/cprograms/mini_project'/stored_data/customer_records.txt", O_RDONLY);

    if (fd == -1 && errno == ENOENT)
    {
        // Customer file was never created
        curr_customer.ID = 0;
    }
    else if (fd == -1)
    {
        perror("Error while opening customer records file(admin_fun.h)");
        exit(1);
    }
    else{
        int offset = lseek(fd, -sizeof(struct Customer), SEEK_END);
         if (offset == -1)
        {
            perror("Error seeking to last Customer record!");
            exit(1);
        }
        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};
        int status = fcntl(fd, F_SETLKW, &lock);

          if (status == -1)
        {
            perror("Error obtaining read lock on Customer record!");
            exit(1);
        }
         rb = read(fd, &prev_customer, sizeof(struct Customer));
        if (rb == -1)
        {
            perror("Error while reading Customer record from file!");
            exit(1);
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);

        curr_customer.ID = prev_customer.ID + 1;
    }



//-----------------------------------------------NAME--------------------------------------------------------------

    sprintf(write_buffer, "%s", ADMIN_ADD_CUSTOMER_NAME);
    wb = write(connection_fd, write_buffer, sizeof(write_buffer));
    if (wb == -1)
    {
        perror("Error writing ADMIN_ADD_CUSTOMER_NAME message to client!");
        exit(1);
    }

    rb = read(connection_fd, read_buffer, sizeof(read_buffer));
    if (rb == -1)
    {
        perror("Error reading customer name response from client!");
        exit(1);
    }

    strcpy(curr_customer.name, read_buffer);


//--------------------------------------------GENDER---------------------------------------------------------
    
     wb = write(connection_fd, ADMIN_ADD_CUSTOMER_GENDER, strlen(ADMIN_ADD_CUSTOMER_GENDER));
    if (wb == -1)
    {
        perror("Error writing ADMIN_ADD_CUSTOMER_GENDER message to client!");
        exit(1);
    }

    bzero(read_buffer, sizeof(read_buffer));
    rb = read(connection_fd, read_buffer, sizeof(read_buffer));
    if (rb == -1)
    {
        perror("Error reading customer gender response from client!");
        exit(1);
    }

    if (read_buffer[0] == 'M' || read_buffer[0] == 'F' || read_buffer[0] == 'O')
        curr_customer.gender = read_buffer[0];
    else
    {
        wb = write(connection_fd, ADMIN_ADD_CUSTOMER_WRONG_GENDER, strlen(ADMIN_ADD_CUSTOMER_WRONG_GENDER));
        rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read
        exit(1);
    }



//----------------------------------------------AGE-----------------------------------------------------------


     bzero(write_buffer, sizeof(write_buffer));
    strcpy(write_buffer, ADMIN_ADD_CUSTOMER_AGE);
    wb = write(connection_fd, write_buffer, strlen(write_buffer));
    if (wb == -1)
    {
        perror("Error writing ADMIN_ADD_CUSTOMER_AGE message to client!");
        exit(1);
    }

    bzero(read_buffer, sizeof(read_buffer));
    rb = read(connection_fd, read_buffer, sizeof(read_buffer));
    if (rb == -1)
    {
        perror("Error reading customer age response from client!");
        exit(1);
    }
     int customer_age = atoi(read_buffer);
      curr_customer.age = customer_age;
    

    //----------------------------------------------Account Number-----------------------------------------------------------


     bzero(write_buffer, sizeof(write_buffer));
    strcpy(write_buffer, ADMIN_ADD_ACCOUNT_NUMBER);
    wb = write(connection_fd, write_buffer, strlen(write_buffer));
    if (wb == -1)
    {
        perror("Error writing ADMIN_ADD_ACCOUNT_NUMBER message to client!");
        exit(1);
    }

    bzero(read_buffer, sizeof(read_buffer));
    rb = read(connection_fd, read_buffer, sizeof(read_buffer));
    if (rb == -1)
    {
        perror("Error reading customer age response from client!");
        exit(1);
    }
     int customer_acc_no = atoi(read_buffer);
      curr_customer.account_no = customer_acc_no;
    
 
    //----------------------------------------------Email Address-----------------------------------------------------------   
 bzero(write_buffer, sizeof(write_buffer));
    strcpy(write_buffer, ADMIN_ADD_EMAIL_ID);
    wb = write(connection_fd, write_buffer, strlen(write_buffer));
    if (wb == -1)
    {
        perror("Error writing ADMIN_ADD_EMAIL_ID message to client!");
        exit(1);
    }

    bzero(read_buffer, sizeof(read_buffer));
    rb = read(connection_fd, read_buffer, sizeof(read_buffer));
    if (rb == -1)
    {
        perror("Error reading customer age response from client!");
        exit(1);
    }
     char * customer_email_add = read_buffer;
     strcpy(curr_customer.email , customer_email_add);
    //----------------------LOGIN---------------------------
     strcpy(curr_customer.login,customer_email_add);
     


    //------------------hashed password-------------------------
    //char hashedPassword[1000];
    // strcpy(hashedPassword, crypt(AUTOGEN_PASSWORD, SALT_BAE));
    // strcpy(newCustomer.password, hashedPassword);
    // wb = write(connection_fd, , strlen());

      unsigned char hash_pass[EVP_MAX_MD_SIZE];     // Array to hold the resulting hash
      char pass[HASH_SIZE + 1];
    // Hash the password
      snprintf(pass, HASH_SIZE + 1, "%s%d", curr_customer.name, curr_customer.ID);
       hash_password(pass, hash_pass);
      strcpy(curr_customer.password,hash_pass);
      

    fd = open("/home/aayushi312000/Desktop/Documents/cprograms/mini_project'/stored_data/customer_records.txt", O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    if (fd == -1)
    {
        perror("Error while creating / opening customer file!");
        exit(1);
    }
    wb = write(fd, &curr_customer, sizeof(curr_customer));
    if (wb == -1)
    {
        perror("Error while writing Customer record to file!");
        exit(1);
    }
    close(fd);
    bzero(write_buffer, sizeof(write_buffer));
    //printf("EMAIL====%s",curr_customer.email);
    sprintf(write_buffer, "%s\n%s%s\n%s%s%d\n\n",LOGIN_SUCCESSFULL_MSG, LOGIN_ID_IS, curr_customer.login, LOGIN_PASS_IS, curr_customer.name, curr_customer.ID);
    // strcat(write_buffer, "^");
    wb = write(connection_fd, write_buffer, strlen(write_buffer));
    if (wb == -1)
    {
        perror("Error sending customer loginID and password to the client!");
        exit(1);
    }

    //rb = read(connection_fd, read_buffer, sizeof(read_buffer));
    return;

}

bool admin_operation_handler(int connection_fd)
{
    char read_buffer[1000], write_buffer[1000];
    ssize_t rb, wb;
    int choice;
    // printf("\n\n....in admin operation");
    while (1){
    
        /* code */
    
    wb = write(connection_fd, ADMIN_MENU, strlen(ADMIN_MENU));
    if (wb == -1)
        perror("Error while sending first prompt to the user!(admin_fun)");
    else
    {
        bzero(read_buffer, sizeof(read_buffer)); // empty buffer
        rb = read(connection_fd, read_buffer, sizeof(read_buffer));
        if (rb == -1)
            perror("Error while reading from client(admin_fun)");
        else if (rb == 0)
            printf("No data was sent by the client(admin_fun)");
        else
        {
            choice = atoi(read_buffer);
            switch (choice)
            {
            case 1:
                // Admin
                printf("add customers:");
                add_customers(connection_fd);
                printf("add customers@@@:");
                break;
            // case 2:
            //     // Customer
            //          printf("customer LOGIN:");
            //     //customer_operation_handler(connection_fd);
            //     break;
            //     case 3:
            //     // Customer
            //       printf("manager LOGIN:");
            //     //customer_operation_handler(connection_fd);
            //     break;
            //     case 4:
            //     // Customer
            //          printf("bank employee LOGIN:");
            //     //customer_operation_handler(connection_fd);
            //     break;
            case 8:
            printf("exit");
            default:
                // Exit
                break;

            }
            if(choice==8){
                break;
            }
        }
    }
    // if (login_handler(true, connFD, NULL))
    // {

    // }
    //  else
    // {
    //     // ADMIN LOGIN FAILED
    //     exit(1);
    // }
}
    return 1;
}

// void add_customer()
#endif