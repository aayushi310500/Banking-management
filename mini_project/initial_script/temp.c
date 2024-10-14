#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

#include "../header_files/data.h"
#include "../common_functions/hash_password.h"
#include "../struct_files/admin.h"

#define MAX_LINE 1024
#define HASH_HEX_SIZE 64  // SHA-256 produces 64 hex characters

int authenticate_admin(const char *input_login, const char *input_pass) {
    struct Admin admin;
    char read_buffer[MAX_LINE];

    // Open the file that stores the admin login ID and hashed password
    int fd = open("/home/aayushi312000/MTech/SS/Banking-management/mini_project/stored_data/admin_password.txt", O_RDONLY);
    if (fd == -1) {
        perror("Error while opening admin password file");
        return -1;
    }

    // Read the file content
    ssize_t bytes_read = read(fd, read_buffer, sizeof(read_buffer) - 1);
    if (bytes_read == -1) {
        perror("Error while reading admin password file");
        close(fd);
        return -1;
    }
    read_buffer[bytes_read] = '\0';  // Null-terminate the read data

    // Close the file
    close(fd);

    // Parse the file content to extract the login ID and hashed password
    char stored_login[MAX_LINE];
    char stored_hash[HASH_HEX_SIZE + 1];  // Only store the first 64 hex characters
    sscanf(read_buffer, "Login ID: %s\nPassword (hashed): %64s\n", stored_login, stored_hash);
    stored_hash[HASH_HEX_SIZE] = '\0';  // Ensure null termination of the hash

    // Check if the login ID matches
    if (strcmp(input_login, stored_login) != 0) {
        printf("Login ID does not match!\n");
        return 0;  // Login failed
    }

    // Hash the input password using the same hash_password function
    unsigned char hash_pass[EVP_MAX_MD_SIZE];  // Buffer for the hashed password
    hash_password(input_pass, hash_pass);

    // Convert the hashed password to hexadecimal string
    char hex_hash[HASH_HEX_SIZE + 1];
    for (int i = 0; i < EVP_MD_size(EVP_sha256()); i++) {  // Use the size for SHA-256
        sprintf(&hex_hash[i * 2], "%02x", hash_pass[i]);
    }
    hex_hash[HASH_HEX_SIZE] = '\0';  // Null-terminate the string

    // Compare the hashed password with the stored hash
    if (strcmp(hex_hash, stored_hash) == 0) {
        printf("Authentication successful!\n");
        return 1;  // Authentication success
    } else {
        printf("Password does not match!\n");
        return 0;  // Authentication failed
    }
    return 1;
}

void main() {
    authenticate_admin("admin123@gmail.com", "admin122");
}
