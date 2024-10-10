#include <stdio.h>      // Import for `printf` & `perror` functions
#include <errno.h>      // Import for `errno` variable
#include <fcntl.h>      // Import for `fcntl` functions
#include <unistd.h>     // Import for `fork`, `fcntl`, `read`, `write`, `lseek, `_exit` functions
#include <sys/types.h>  // Import for `socket`, `bind`, `listen`, `connect`, `fork`, `lseek` functions
#include <sys/socket.h> // Import for `socket`, `bind`, `listen`, `connect` functions
#include <netinet/ip.h> // Import for `sockaddr_in` stucture
#include <string.h>     // Import for string functions
#include <stdlib.h>     //Import exit(1)

#include "header_files/data.h"

#define BUFFER_SIZE 1024

void connection_handler(int socket_fd)
{
    char read_buffer[1000], write_buffer[1000]; // A buffer used for reading from / writting to the server
    ssize_t rb, wb;                             // Number of bytes read from / written to the socket

    char buff[1000];

    do
    {
    bzero(read_buffer, sizeof(read_buffer)); // Empty the read buffer
    bzero(buff, sizeof(buff));               // empty buff
    rb = read(socket_fd, read_buffer, sizeof(read_buffer));
    if (rb == -1)
        perror("Error while reading from client socket!");
    else if (rb == 0)
        printf("No error received from server! Closing the connection to the server now!\n");
    else
    {
        // Read the welcome message from the server
        // int bytes_read = read(socket_fd, buff, sizeof(buff));
        //  if (bytes_read < 0) {
        // perror("Error reading from server:");
        // close(socket_fd);
        // exit(1);
        // }
        // else{
        // buff[rb] = '\0';  // Null-terminate the string
        // printf("Server: %s\n", buff);
     
            read_buffer[rb] = '\0';  // Null-terminate the read data
            printf("Server: %s\n", read_buffer); // Print server message
            
        // int choice;
        // char str[2];
        // scanf("%d", &choice);

        // sprintf(str, "%d", choice);
        // wb = write(socket_fd, str, strlen(str));
        // if (wb == -1)
        //     perror("Error writing to server:");
        // else
        // {
            // bzero(read_buffer, sizeof(read_buffer)); // Empty the read buffer
            // bzero(buff, sizeof(buff));               // empty buff
            // rb = read(socket_fd, buff, sizeof(read_buffer));
            // if (rb == -1)
            //     perror("Error while reading from client socket!(menu2)");
            // else if (rb == 0)
            //     printf("No error received from server! Closing the connection to the server now!(menu2)\n");
            // else
            // {
            //     // printf("Server: %s\n", buff);
            //     int choice2;
            //     char str2[2];
            //     scanf("%d", &choice2);

            //     sprintf(str, "%d", choice2);
            //     wb = write(socket_fd, str2, strlen(str2));
            //     if (wb == -1)
            //         perror("Error writing to server:");
            // }

            bzero(write_buffer, sizeof(write_buffer)); // Empty the write buffer

            // if (strchr(readBuffer, '#') != NULL)
            //     strcpy(write_buffer, getpass(readBuffer));
            // else
            // {
                //printf("%s\n", readBuffer);
                // printf("scan..in else");
                scanf("%[^\n]%*c", write_buffer); // Take user input!
            // }
             // printf("writing..in else");
            wb = write(socket_fd, write_buffer, strlen(write_buffer));
            if (wb == -1)
            {
                perror("Error while writing to client socket!");
                printf("Closing the connection to the server now!\n");
                break;
            }


        // }
        }
    }

    // else if (strchr(read_buffer, '^') != NULL)
    // {
    //     // Skip read from client
    //     strncpy(buff, read_buffer, strlen(read_buffer) - 1);
    //     printf("%s\n", buff);
    //     wb = write(socket_fd, "^", strlen("^"));
    //     if (wb == -1)
    //     {
    //         perror("Error while writing to client socket!");
    //         break;
    //     }
    // }
    // else if (strchr(read_buffer, '$') != NULL)
    // {
    //     // Server sent an error message and is now closing it's end of the connection
    //     strncpy(buff, read_buffer, strlen(read_buffer) - 2);
    //     printf("%s\n", buff);
    //     printf("Closing the connection to the server now!\n");
    //     break;
    // }
    // else
    // {
    //     bzero(write_buffer, sizeof(write_buffer)); // Empty the write buffer

    //     if (strchr(read_buffer, '#') != NULL)
    //         strcpy(write_buffer, getpass(read_buffer));
    //     else
    //     {
    //         printf("%s\n", read_buffer);
    //         scanf("%[^\n]%*c", write_buffer); // Take user input!
    //     }

    //     wb = write(socket_fd, write_buffer, strlen(write_buffer));
    //     if (wb == -1)
    //     {
    //         perror("Error while writing to client socket!");
    //         printf("Closing the connection to the server now!\n");
    //         break;
    //     }
    // }
     //}
      while (rb > 0);

    close(socket_fd);
}

void main()
{
    int socket_fd, status;
    struct sockaddr_in server_address;
    // struct sockaddr server;
    char buffer[BUFFER_SIZE];

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        perror("Error while creating server socket!");
        exit(1);
    }

    server_address.sin_family = AF_INET;                // IPv4
    server_address.sin_port = htons(PORT_NO);              // Server will listen to port 8080
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); // Binds the socket to all interfaces

    status = connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address));
    if (status == -1)
    {
        perror("Error while connecting to server!");
        close(socket_fd);
        exit(1);
    }

    connection_handler(socket_fd);

    // int bytes_read = read(socket_fd, buffer, BUFFER_SIZE - 1);
    // if (bytes_read < 0) {
    //     perror("Error reading from server:");
    //     close(socket_fd);
    //     exit(1);
    // }
    // buffer[bytes_read] = '\0';  // Null-terminate the string
    // printf("Server: %s\n", buffer);

    // // Send a message to the server
    // printf("Enter message to send to server: ");
    // fgets(buffer, BUFFER_SIZE, stdin);
    // write(socket_fd, buffer, strlen(buffer));

    // Close the socket
    close(socket_fd);
    printf("Connection closed with server.\n");

    close(socket_fd);
}
