#ifndef ASSIGN_FEEDBACK
#define ASSIGN_FEEDBACK

#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

#include "../header_files/data.h"

void show_managers() {
    char read_buffer[1000], write_buffer[1000];
    ssize_t rb, wb;
    // Open the file that stores the admin login ID and hashed password
    int fd = open("/home/aayushi312000/MTech/SS/Banking-management/mini_project/stored_data/manager_records.txt", O_RDONLY);
    if (fd == -1) {
        perror("Error while opening admin password file");
        return -1;
    }
     
    

    // char line[256];
    // printf("Manager ID | Name | Email\n");
    // printf("----------------------------------\n");

    // while (fgets(line, sizeof(line), fp)) {
    //     int manager_id;
    //     char name[100], email[100];

    //     sscanf(line, "%d,%[^,],%s", &manager_id, name, email);
    //     printf("%d | %s | %s\n", manager_id, name, email);
    // }

    // fclose(fp);
}

void assign_feedback_to_manager_console() {
    int feedback_id, manager_id;
    printf("Enter Feedback ID to assign: ");
    scanf("%d", &feedback_id);

    // Show list of managers
    printf("Available Managers:\n");
    show_managers();  // Function to display the list of managers (from file or database)

    printf("Enter Manager ID to assign to: ");
    scanf("%d", &manager_id);

    // Call the function to assign feedback
    assign_feedback_to_manager(feedback_id, manager_id);
}
void assign_feedback_to_manager(int feedback_id, int manager_id) {
    struct Feedback feedback;
    int fd = open("stored_data/feedback.txt", O_RDWR);
    if (fd == -1) {
        perror("Error while opening feedback file");
        return;
    }

    // Search for the feedback by ID
    while (read(fd, &feedback, sizeof(struct Feedback)) > 0) {
        if (feedback.feedback_id == feedback_id) {
            // Update the manager ID
            feedback.manager_id = manager_id;

            // Seek back to the position of this feedback record
            lseek(fd, -sizeof(struct Feedback), SEEK_CUR);

            // Write the updated record
            write(fd, &feedback, sizeof(struct Feedback));
            break;
        }
    }

    close(fd);
}


#endif