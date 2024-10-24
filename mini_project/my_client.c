#include <stdio.h>      
#include <errno.h>      
#include <fcntl.h>     
#include <unistd.h>     
#include <sys/types.h>  
#include <sys/socket.h>
#include <netinet/ip.h> 
#include <string.h>    
#include <stdlib.h>    

#include "header_files/data.h"

#define BUFFER_SIZE 1024


void connection_handler(int socket_fd)
{
    char read_buffer[1000], write_buffer[1000]; 
    ssize_t rb, wb;                            

    char buff[1000];

    do
    {
        // printf("hhhh");
        // fflush(stdout);
        // fflush(stdin);
       bzero(read_buffer, sizeof(read_buffer));   // Empty the read buffer
       bzero(write_buffer, sizeof(write_buffer)); // Empty the read buffer
       bzero(buff, sizeof(buff));                 // empty buff
        // fflush(stdout);
        // fflush(stdin);
        
        rb = read(socket_fd, read_buffer, sizeof(read_buffer));
        // fflush(stdout);
        // fflush(stdin);
       
        if (rb == -1)
        {
            // printf("IN9");
            perror("Error while reading from client socket!");
        }
        else if (rb == 0)
        {
            //   printf("IN10");
            printf("No error received from server! Closing the connection to the server now!");
        }

        else if (strchr(read_buffer, '^') != NULL)
        {
            // printf("IN1");
            // Skip read from client
            // fflush(stdout);
            // fflush(stdin);
            // strncpy(buff, read_buffer, strlen(read_buffer) - 1);
            strncpy(buff, read_buffer, strlen(read_buffer) - 1);
            printf("%s\n", buff);
            wb = write(socket_fd, "^", strlen("^"));
            if (wb == -1)
            {
                perror("Error while writing to client socket!");
                break;
            }
            // fflush(stdout);
            // fflush(stdin);
            // wb = write(socket_fd, "^", strlen("^"));
            // if (wb == -1)
            // {
            //     perror("Error while writing to client socket!");
            //     break;
            // }
        }
        else if (strchr(read_buffer, '$') != NULL)
        {
            // Server sent an error message and is now closing it's end of the connection
            //  printf("IN2");
            // fflush(stdout);
            // fflush(stdin);
            // strncpy(buff, read_buffer, strlen(read_buffer) - 2);
            printf("%s", read_buffer);
            printf("Closing the connection to the server now!");
            // fflush(stdout);
            // fflush(stdin);
            break;
        }
        else if (strchr(read_buffer, '@') != NULL)
        {
            // printf("I-----N");
            // fflush(stdout);
            // fflush(stdin);
            //printf("IN3");
            // fflush(stdout);
            // fflush(stdin);
            printf("%s\n", read_buffer);
           // bzero(read_buffer, sizeof(read_buffer));
            // fflush(stdout);
            // fflush(stdin);
           // printf("IN3");
           // continue;
        }
        else
        {
            //printf("IN4");
            bzero(write_buffer, sizeof(write_buffer)); // Empty the write buffer

            if (strchr(read_buffer, '#') != NULL)
                strcpy(write_buffer, getpass(read_buffer));
            else
            {
                // fflush(stdout);
                // fflush(stdin);
               // printf("IN5");
                printf("%s", read_buffer);
                // fflush(stdout);
                // fflush(stdin);
                // printf("why");
                 scanf("%[^\n]%*c", write_buffer); // Take user input!
                //fgets(write_buffer, sizeof(write_buffer), stdin);
                // fflush(stdout);
                // fflush(stdin);
                // fgets(write_buffer, sizeof(write_buffer), stdin);
                // write_buffer[strcspn(write_buffer, "\n")] = 0;
                //  fflush(stdin);

               // printf("You entered: '%s'", write_buffer);
                // fflush(stdout);
                // fflush(stdin);
            }
            if (write_buffer)
            {
                //printf("IN6");
                wb = write(socket_fd, write_buffer, strlen(write_buffer));

                if (wb == -1) {
                    perror("Error while writing to client socket!");
                    printf("Closing the connection to the server now!\n");
                    break;
                }
                //  printf("done writng from the client side..");
            }
            // fflush(stdout);
        }

        // else
        // {
        //     // Read the welcome message from the server

        //         read_buffer[rb] = '\0';  // Null-terminate the read data
        //         printf("Server: %s\n", read_buffer); // Print server message

        //         bzero(write_buffer, sizeof(write_buffer)); // Empty the write buffer

        //             scanf("%[^\n]%*c", write_buffer); // Take user input!

        //           printf("writing..in else myclient");
        //         wb = write(socket_fd, write_buffer, strlen(write_buffer));
        //         if (wb == -1)
        //         {
        //             perror("Error while writing to client socket!");
        //             printf("Closing the connection to the server now!\n");
        //             break;
        //         }

        //     }
        // fflush(stdout);
        // fflush(stdin);
        // bzero(read_buffer, sizeof(read_buffer));   // Empty the read buffer
        // bzero(write_buffer, sizeof(write_buffer)); // Empty the read buffer
        // bzero(buff, sizeof(buff));                 // empty buff

    }

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
    server_address.sin_port = htons(PORT_NO);           // Server will listen to port 8080
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