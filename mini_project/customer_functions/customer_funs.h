#ifndef CUSTOMER_FUNCTIONS_H
#define CUSTOMER_FUNCTIONS_H



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

#include "login_customer.h"
struct Customer loggedInCustomer;

bool customer_operation_handler(int connection_fd){
      bool ans;
    ans = login_handler_customer(connection_fd,&loggedInCustomer);
    printf("ans:-%d",ans);
}

#endif