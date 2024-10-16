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
#include "header_files/data.h"
#include "common_functions/admin_fun.h"
#include "customer_functions/customer_funs.h"


void handle_client(int connection_fd) {
     char read_buffer[1000], write_buffer[1000];
    ssize_t rb, wb;
    int choice;
    wb = write(connection_fd, INITIAL_PROMPT, strlen(INITIAL_PROMPT));
    if (wb == -1)   
        perror("Error while sending first prompt to the user!");
    else{
         bzero(read_buffer, sizeof(read_buffer)); //empty buffer
          rb = read(connection_fd, read_buffer, sizeof(read_buffer));
        if (rb == -1)
            perror("Error while reading from client");
        else if (rb == 0)
            printf("No data was sent by the client");
        else{
             choice = atoi(read_buffer);
            switch (choice)
            {
            case 1:
                // Admin
                printf("admin LOGIN:");
                admin_operation_handler(connection_fd);
                break;
            case 2:
                // Customer
                     printf("customer LOGIN:");
                     customer_operation_handler(connection_fd);
                //customer_operation_handler(connection_fd);
                break;
                case 3:
                // Customer
                     printf("manager LOGIN:");
                //customer_operation_handler(connection_fd);
                break;
                case 4:
                // Customer
                     printf("bank employee LOGIN:");
                //customer_operation_handler(connection_fd);
                break;
            default:
                // Exit
                break;
            }
        }
         
    }

    // Close the connection after communication
    close(connection_fd);
}


void main()
{
    int socket_fd, connection_fd,status;
    struct sockaddr_in server_address, client_address;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        perror("Error while creating server socket!");
        exit(1);
    }

    server_address.sin_family = AF_INET;                // IPv4
    server_address.sin_port = htons(PORT_NO);              // Server will listen to port 8080
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); // Binds the socket to all interfaces

    status = bind(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address));
    if (status == -1)
    {
        perror("Error while binding to server socket!");
        exit(1);
    }

    status = listen(socket_fd, 10);
    if (status == -1)
    {
        perror("Error while listening for connections on the server socket!");
        close(socket_fd);
        exit(1);
    }

    int clientadd_size;
    while (1)
    {
        clientadd_size = (int)sizeof(client_address);
        connection_fd = accept(socket_fd, (struct sockaddr *)&client_address, &clientadd_size);
        if (connection_fd == -1)
        {
            perror("Error while connecting to client!");
            close(connection_fd);
        }
        else
        {
            if (!fork())
            {
                // Child will enter this branch
               // connection_handler(connectionFileDescriptor);
                handle_client(connection_fd);
                close(socket_fd);
                exit(1);
            }
        }
    }

    close(socket_fd);
}
