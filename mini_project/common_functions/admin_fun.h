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
#include "../struct_files/transaction.h"
#include "../struct_files/account.h"
#include "hash_password.h"
#include "login.h"



bool get_customer_details(int connection_fd, int customer_id);
bool modify_customer_details(int connection_fd);



int add_customers(int connection_fd,bool from_account,int account_num)
{
    char read_buffer[1000], write_buffer[1000];
    ssize_t rb, wb;
    struct Customer curr_customer, prev_customer;

    int fd = open(CUSTOMER_FILE, O_RDONLY);

    if (fd == -1 && errno == ENOENT)
    {
        // Customer file was never created
        curr_customer.ID = 0;
    }
    else if (fd == -1)
    {
        perror("Error while opening customer records file(admin_fun.h)");
       return -1;
    }
    else{
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
         rb = read(fd, &prev_customer, sizeof(struct Customer));
        if (rb == -1)
        {
            perror("Error while reading Customer record from file!");
            return false;
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);

        curr_customer.ID = prev_customer.ID + 1;
    }



//-----------------------------------------------NAME--------------------------------------------------------------

    sprintf(write_buffer, "%s", ADMIN_ADD_CUSTOMER_NAME);
    wb = write(connection_fd, write_buffer, strlen(write_buffer));//change
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

    strcpy(curr_customer.name, read_buffer);


//--------------------------------------------GENDER---------------------------------------------------------
    
     wb = write(connection_fd, ADMIN_ADD_CUSTOMER_GENDER, strlen(ADMIN_ADD_CUSTOMER_GENDER));
    if (wb == -1)
    {
        perror("Error writing ADMIN_ADD_CUSTOMER_GENDER message to client!");
        return false;
    }

    bzero(read_buffer, sizeof(read_buffer));
    rb = read(connection_fd, read_buffer, sizeof(read_buffer));
    if (rb == -1)
    {
        perror("Error reading customer gender response from client!");
        return false;
    }

    if (read_buffer[0] == 'M' || read_buffer[0] == 'F' || read_buffer[0] == 'O')
        curr_customer.gender = read_buffer[0];
    else
    {
        wb = write(connection_fd, ADMIN_ADD_CUSTOMER_WRONG_GENDER, strlen(ADMIN_ADD_CUSTOMER_WRONG_GENDER));
        rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read
       return false;
    }



//----------------------------------------------AGE-----------------------------------------------------------


     bzero(write_buffer, sizeof(write_buffer));
    strcpy(write_buffer, ADMIN_ADD_CUSTOMER_AGE);
    wb = write(connection_fd, write_buffer, strlen(write_buffer));
    if (wb == -1)
    {
        perror("Error writing ADMIN_ADD_CUSTOMER_AGE message to client!");
        return false;
    }

    bzero(read_buffer, sizeof(read_buffer));
    rb = read(connection_fd, read_buffer, sizeof(read_buffer));
    if (rb == -1)
    {
        perror("Error reading customer age response from client!");
        return false;
    }
     int customer_age = atoi(read_buffer);
      curr_customer.age = customer_age;
    

    //----------------------------------------------Account Number-----------------------------------------------------------


    if(!from_account){
          bzero(write_buffer, sizeof(write_buffer));
    strcpy(write_buffer, ADMIN_ADD_ACCOUNT_NUMBER);
    wb = write(connection_fd, write_buffer, strlen(write_buffer));
    if (wb == -1)
    {
        perror("Error writing ADMIN_ADD_ACCOUNT_NUMBER message to client!");
       return false;
    }

    bzero(read_buffer, sizeof(read_buffer));
    rb = read(connection_fd, read_buffer, sizeof(read_buffer));
    if (rb == -1)
    {
        perror("Error reading customer age response from client!");
        return false;
    }
     int customer_acc_no = atoi(read_buffer);
      curr_customer.account_no = customer_acc_no;
    }
   else{
     curr_customer.account_no = account_num;
   }
     
    
 
    //----------------------------------------------Email Address-----------------------------------------------------------   
 bzero(write_buffer, sizeof(write_buffer));
    strcpy(write_buffer, ADMIN_ADD_EMAIL_ID);
    wb = write(connection_fd, write_buffer, strlen(write_buffer));
    if (wb == -1)
    {
        perror("Error writing ADMIN_ADD_EMAIL_ID message to client!");
        return false;
    }

    bzero(read_buffer, sizeof(read_buffer));
    rb = read(connection_fd, read_buffer, sizeof(read_buffer));
    if (rb == -1)
    {
        perror("Error reading customer age response from client!");
        return false;
    }
     char * customer_email_add = read_buffer;
     strcpy(curr_customer.email , customer_email_add);
    //-----------------------------------------------------LOGIN---------------------------
     strcpy(curr_customer.login,customer_email_add);
     


    //---------------------------------------------------------hashed password-------------------------
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
      

    fd = open(CUSTOMER_FILE, O_CREAT | O_APPEND | O_WRONLY, 0777);
    if (fd == -1)
    {
        perror("Error while creating / opening customer file!");
        return false;
    }
    wb = write(fd, &curr_customer, sizeof(curr_customer));
    if (wb == -1)
    {
        perror("Error while writing Customer record to file!");
       return false;
    }
    close(fd);
    bzero(write_buffer, strlen(write_buffer)); //change
    //printf("EMAIL====%s",curr_customer.email);
    sprintf(write_buffer, "%s\n%s%s\n%s%s%d\n\n",LOGIN_SUCCESSFULL_MSG, LOGIN_ID_IS, curr_customer.login, LOGIN_PASS_IS, curr_customer.name, curr_customer.ID);
    // strcat(write_buffer, "^");
    wb = write(connection_fd, write_buffer, strlen(write_buffer));
    if (wb == -1)
    {
        perror("Error sending customer loginID and password to the client!");
        return false;
    }


 // rb = read(connection_fd, read_buffer, sizeof(read)); // Dummy read

    bzero(write_buffer, strlen(write_buffer));
   // sprintf(write_buffer, "%s%d", ADMIN_ADD_ACCOUNT_NUMBER, curr_account.account_number);
    strcat(write_buffer, "\nRedirecting you to the main menu ...^");
    wb = write(connection_fd, write_buffer, strlen(write_buffer));
    rb = read(connection_fd, read_buffer, sizeof(read)); // Dummy read



    return curr_customer.ID;

}


bool admin_operation_handler(int connection_fd)
{
    bool ans;
    ans=login_handler(true,connection_fd,NULL);
    //printf("-------%d",ans);
    if(ans){
     //   printf("\n\n..===============..in admin operation=========================");
    char read_buffer[1000], write_buffer[1000];
    ssize_t rb, wb;
    int choice;
   
   // printf("\n\n..===============..in admin operation=========================");
    while (1){
    
        /* code */
    
    wb = write(connection_fd, ADMIN_MENU, strlen(ADMIN_MENU));
    if (wb == -1)
        perror("Error while sending first prompt to the user!(admin_fun)");
    else
    {
        
        bzero(read_buffer, sizeof(read_buffer)); // empty buffer
       //printf("======================================================%s",read_buffer);
        rb = read(connection_fd, read_buffer, sizeof(read_buffer));
        
        if (rb == -1)
            perror("Error while reading from client(admin_fun)");
        else if (rb == 0)
            printf("No data was sent by the client(admin_fun)");
        else
        {    
            // perror("==================================in else loop====================");
            choice = atoi(read_buffer);
            switch (choice)
            {
            case 1:
                // Admin
                // printf("add customers. begin:");
                add_customers(connection_fd,0,-1);
                // printf("add customers end");
                break;
            case 2:
                // Customer
                     //printf("customer LOGIN:");

                      get_customer_details(connection_fd,-1);
                      break;
            case 7:
               modify_customer_details(connection_fd);
               break;
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
            printf("closing connection");
               wb = write(connection_fd, CUSTOMER_LOGOUT, strlen(CUSTOMER_LOGOUT));
                return false;
            // close(connection_fd); // Close connection
            // return true;
            default:
                printf("Invalid choice, returning to menu.");
                break;

            }
            
        }
        if(choice==8){
                break;
            }
    }
    // if (login_handler(true, connection_fd, NULL))
    // {

    // }
    //  else
    // {
    //     // ADMIN LOGIN FAILED
    //     exit(1);
    // }
}
}
else{

    return 0;
}
    return 1;
}

// void add_customer()

bool add_account(int connection_fd)
{
    ssize_t rb, wb;
    char read_buffer[1000], write_buffer[1000];

    struct Account curr_account, prev_account;

    int fd = open(ACCOUNT_FILE, O_RDONLY);
    if (fd == -1 && errno == ENOENT)
    {
        // Account file was never created
        curr_account.account_number = 0;
    }
    else if (fd == -1)
    {
        perror("Error while opening account file");
        return false;
    }
    else
    {
        int offset = lseek(fd, -sizeof(struct Account), SEEK_END);
        if (offset == -1)
        {
            perror("Error seeking to last Account record!");
            return false;
        }

        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Account), getpid()};
        int status = fcntl(fd, F_SETLKW, &lock);
        if (status == -1)
        {
            perror("Error obtaining read lock on Account record!");
            return false;
        }

        rb = read(fd, &prev_account, sizeof(struct Account));
        if (rb == -1)
        {
            perror("Error while reading Account record from file!");
            return false;
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);

        curr_account.account_number = prev_account.account_number + 1;
    }
    // wb = write(connection_fd, ADMIN_ADD_ACCOUNT_TYPE, strlen(ADMIN_ADD_ACCOUNT_TYPE));
    // if (wb == -1)
    // {
    //     perror("Error writing ADMIN_ADD_ACCOUNT_TYPE message to client!");
    //     return false;
    // }

    // bzero(read_buffer, sizeof(read_buffer));
    // rb = read(connection_fd, &read_buffer, sizeof(read_buffer));
    // if (rb == -1)
    // {
    //     perror("Error reading account type response from client!");
    //     return false;
    // }

    //curr_account.isRegularAccount = atoi(read_buffer) == 1 ? true : false;

    curr_account.customer_id = add_customers(connection_fd, true, curr_account.account_number);

    // if (curr_account.isRegularAccount)
    //     curr_account.owners[1] = -1;
    // else
    //     curr_account.owners[1] = add_customer(connection_fd, false, curr_account.account_number);

    curr_account.is_active = true;
    curr_account.balance = 0;

    memset(curr_account.transactions, -1, MAX_TRANSACTIONS * sizeof(int));

    fd = open(ACCOUNT_FILE, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    if (fd == -1)
    {
        perror("Error while creating / opening account file!");
        return false;
    }

    wb = write(fd, &curr_account, sizeof(struct Account));
    if (wb == -1)
    {
        perror("Error while writing Account record to file!");
        return false;
    }

    close(fd);
    bzero(write_buffer, sizeof(write_buffer));
    sprintf(write_buffer, "%s%d", ADMIN_ADD_ACCOUNT_NUMBER, curr_account.account_number);
    strcat(write_buffer, "\nRedirecting you to the main menu ...^");
    wb = write(connection_fd, write_buffer, sizeof(write_buffer));
    rb = read(connection_fd, read_buffer, sizeof(read)); // Dummy read
    return true;
}

bool get_customer_details(int connection_fd, int customer_id)
{
    ssize_t rb, wb;             // Number of bytes read from / written to the socket
    char read_buffer[1000], write_buffer[10000]; // A buffer for reading from / writing to the socket
    struct Customer customer;
    int fd;
    struct flock lock = {F_RDLCK, SEEK_SET, 0, sizeof(struct Customer), getpid()};
    bool customer_found = false;

    // If customer_id is -1, prompt client for a valid ID
    if (customer_id == -1)
    {
        wb = write(connection_fd, GET_CUSTOMER_ID_MSG, strlen(GET_CUSTOMER_ID_MSG));
        if (wb == -1)
        {
            perror("Error while writing GET_CUSTOMER_ID_MSG message to client!");
            return false;
        }

        bzero(read_buffer, sizeof(read_buffer));
        rb = read(connection_fd, read_buffer, sizeof(read_buffer));
        if (rb == -1)
        {
            perror("Error getting customer ID from client!");
            return false;
        }

        customer_id = atoi(read_buffer);
    }   

    // Open the customer file for reading
    fd = open(CUSTOMER_FILE, O_RDONLY);
    if (fd == -1)
    {
        // Customer File doesn't exist
        bzero(write_buffer, sizeof(write_buffer));
        strcpy(write_buffer, CUSTOMER_ID_DOESNT_EXIT);
        strcat(write_buffer, "^");
        wb = write(connection_fd, write_buffer, strlen(write_buffer));
        if (wb == -1)
        {
            perror("Error while writing CUSTOMER_ID_DOESNT_EXIT message to client!");
        }
        return false;
    }

    // Read through the customer file to find the matching customer_id
    while (read(fd, &customer, sizeof(struct Customer)) > 0)
    {
        if (customer.ID == customer_id)
        {
            // Found the customer, apply a read lock
            lock.l_start = lseek(fd, -sizeof(struct Customer), SEEK_CUR);
            if (fcntl(fd, F_SETLKW, &lock) == -1)
            {
                perror("Error while obtaining read lock on the Customer file!");
                close(fd);
                return false;
            }

            customer_found = true;
            break;
        }
    }

    if (!customer_found)
    {
        // Customer record not found
        bzero(write_buffer, strlen(write_buffer));
        strcpy(write_buffer, CUSTOMER_ID_DOESNT_EXIT);
        strcat(write_buffer, "^");
        wb = write(connection_fd, write_buffer, strlen(write_buffer));
        if (wb == -1)
        {
            perror("Error while writing CUSTOMER_ID_DOESNT_EXIT message to client!");
        }
        close(fd);
        return false;
    }

    // Unlock the file after reading
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);

    // Prepare and send customer details to the client
    bzero(write_buffer, sizeof(write_buffer));
    sprintf(write_buffer, "Customer Details - \n\tID : %d\n\tName : %s\n\tGender : %c\n\tAge: %d\n\tAccount Number : %d\n\tLoginID : %s",
            customer.ID, customer.name, customer.gender, customer.age, customer.account_no, customer.login);

    strcat(write_buffer, "\n\nYou'll now be redirected to the main menu...^");
    wb = write(connection_fd, write_buffer, strlen(write_buffer));
    if (wb == -1)
    {
        perror("Error writing customer info to client!");
        return false;
    }

    // Dummy read to sync with client
    rb = read(connection_fd, read_buffer, sizeof(read_buffer)); 
    return true;
}


bool modify_customer_details(int connection_fd)
{
    ssize_t rb, wb;
    char read_buffer[1000], write_buffer[1000];

    struct Customer customer;

    int customer_id;

    off_t offset;
    int status;

    wb = write(connection_fd, ADMIN_MOD_CUSTOMER_ID, strlen(ADMIN_MOD_CUSTOMER_ID));
    if (wb == -1)
    {
        perror("Error while writing ADMIN_MOD_CUSTOMER_ID message to client!");
        return false;
    }
    bzero(read_buffer, sizeof(read_buffer));
    rb = read(connection_fd, read_buffer, sizeof(read_buffer));
    if (rb == -1)
    {
        perror("Error while reading customer ID from client!");
        return false;
    }

    customer_id = atoi(read_buffer);

    int fd = open(CUSTOMER_FILE, O_RDONLY);
    if (fd == -1)
    {
        // Customer File doesn't exist
        bzero(write_buffer, sizeof(write_buffer));
        strcpy(write_buffer, CUSTOMER_ID_DOESNT_EXIT);
        strcat(write_buffer, "^");
        wb = write(connection_fd, write_buffer, strlen(write_buffer));
        if (wb == -1)
        {
            perror("Error while writing CUSTOMER_ID_DOESNT_EXIT message to client!");
            return false;
        }
        rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read
        return false;
    }
    
    offset = lseek(fd, customer_id * sizeof(struct Customer), SEEK_SET);
    if (errno == EINVAL)
    {
        // Customer record doesn't exist
        bzero(write_buffer, sizeof(write_buffer));
        strcpy(write_buffer, CUSTOMER_ID_DOESNT_EXIT);
        strcat(write_buffer, "^");
        wb = write(connection_fd, write_buffer, strlen(write_buffer));
        if (wb == -1)
        {
            perror("Error while writing CUSTOMER_ID_DOESNT_EXIT message to client!");
            return false;
        }
        rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read
        return false;
    }
    else if (offset == -1)
    {
        perror("Error while seeking to required customer record!");
        return false;
    }

    struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};

    // Lock the record to be read
    status = fcntl(fd, F_SETLKW, &lock);
    if (status == -1)
    {
        perror("Couldn't obtain lock on customer record!");
        return false;
    }

    rb = read(fd, &customer, sizeof(struct Customer));
    if (rb == -1)
    {
        perror("Error while reading customer record from the file!");
        return false;
    }

    // Unlock the record
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);

    wb = write(connection_fd, ADMIN_MOD_CUSTOMER_MENU, strlen(ADMIN_MOD_CUSTOMER_MENU));
    if (wb == -1)
    {
        perror("Error while writing ADMIN_MOD_CUSTOMER_MENU message to client!");
        return false;
    }
    rb = read(connection_fd, read_buffer, sizeof(read_buffer));
    if (rb == -1)
    {
        perror("Error while getting customer modification menu choice from client!");
        return false;
    }

    int choice = atoi(read_buffer);
    if (choice == 0)
    { // A non-numeric string was passed to atoi
        bzero(write_buffer, sizeof(write_buffer));
        strcpy(write_buffer, ERRON_INPUT_FOR_NUMBER);
        wb = write(connection_fd, write_buffer, strlen(write_buffer));
        if (wb == -1)
        {
            perror("Error while writing ERRON_INPUT_FOR_NUMBER message to client!");
            return false;
        }
        rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read
        return false;
    }

    bzero(read_buffer, sizeof(read_buffer));
    switch (choice)
    {
    case 1:
        wb = write(connection_fd, ADMIN_MOD_CUSTOMER_NEW_NAME, strlen(ADMIN_MOD_CUSTOMER_NEW_NAME));
        if (wb == -1)
        {
            perror("Error while writing ADMIN_MOD_CUSTOMER_NEW_NAME message to client!");
            return false;
        }
        rb = read(connection_fd, &read_buffer, sizeof(read_buffer));
        if (rb == -1)
        {
            perror("Error while getting response for customer's new name from client!");
            return false;
        }
        strcpy(customer.name, read_buffer);
        break;
    case 2:
        wb = write(connection_fd, ADMIN_MOD_CUSTOMER_NEW_AGE, strlen(ADMIN_MOD_CUSTOMER_NEW_AGE));
        if (wb == -1)
        {
            perror("Error while writing ADMIN_MOD_CUSTOMER_NEW_AGE message to client!");
            return false;
        }
        rb = read(connection_fd, &read_buffer, sizeof(read_buffer));
        if (rb == -1)
        {
            perror("Error while getting response for customer's new age from client!");
            return false;
        }
        int updatedAge = atoi(read_buffer);
        if (updatedAge == 0)
        {
            // Either client has sent age as 0 (which is invalid) or has entered a non-numeric string
            bzero(write_buffer, sizeof(write_buffer));
            strcpy(write_buffer, ERRON_INPUT_FOR_NUMBER);
            wb = write(connection_fd, write_buffer, strlen(write_buffer));
            if (wb == -1)
            {
                perror("Error while writing ERRON_INPUT_FOR_NUMBER message to client!");
                return false;
            }
            rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read
            return false;
        }
        customer.age = updatedAge;
        break;
    case 3:
        wb = write(connection_fd, ADMIN_MOD_CUSTOMER_NEW_GENDER, strlen(ADMIN_MOD_CUSTOMER_NEW_GENDER));
        if (wb == -1)
        {
            perror("Error while writing ADMIN_MOD_CUSTOMER_NEW_GENDER message to client!");
            return false;
        }
        rb = read(connection_fd, &read_buffer, sizeof(read_buffer));
        if (rb == -1)
        {
            perror("Error while getting response for customer's new gender from client!");
            return false;
        }
        customer.gender = read_buffer[0];
        break;
    default:
        bzero(write_buffer, sizeof(write_buffer));
        strcpy(write_buffer, INVALID_MENU_CHOICE);
        wb = write(connection_fd, write_buffer, strlen(write_buffer));
        if (wb == -1)
        {
            perror("Error while writing INVALID_MENU_CHOICE message to client!");
            return false;
        }
        rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read
        return false;
    }

    fd = open(CUSTOMER_FILE, O_WRONLY);
    if (fd == -1)
    {
        perror("Error while opening customer file");
        return false;
    }
    offset = lseek(fd, customer_id * sizeof(struct Customer), SEEK_SET);
    if (offset == -1)
    {
        perror("Error while seeking to required customer record!");
        return false;
    }

    lock.l_type = F_WRLCK;
    lock.l_start = offset;
    status = fcntl(fd, F_SETLKW, &lock);
    if (status == -1)
    {
        perror("Error while obtaining write lock on customer record!");
        return false;
    }

    wb = write(fd, &customer, sizeof(struct Customer));
    if (wb == -1)
    {
        perror("Error while writing update customer info into file");
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);

    close(fd);

    wb = write(connection_fd, ADMIN_MOD_CUSTOMER_SUCCESS, strlen(ADMIN_MOD_CUSTOMER_SUCCESS));
    if (wb == -1)
    {
        perror("Error while writing ADMIN_MOD_CUSTOMER_SUCCESS message to client!");
        return false;
    }
    rb = read(connection_fd, read_buffer, sizeof(read_buffer)); // Dummy read

    return true;
}
#endif  