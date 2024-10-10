// #ifndef ADMIN_PASSWORD
// #define ADMIN_PASSWORD

// #include <stdio.h>     // Import for `printf` & `perror`
// #include <unistd.h>    // Import for `read`, `write & `lseek`
// #include <string.h>    // Import for string functions
// #include <stdbool.h>   // Import for `bool` data type
// #include <sys/types.h> // Import for `open`, `lseek`
// #include <sys/stat.h>  // Import for `open`
// #include <fcntl.h>     // Import for `open`
// #include <stdlib.h>    // Import for `atoi`
// #include <errno.h>     // Import for `errno`
// #include <sodium.h>

// // #include "../header_files/data.h"
// // #include "../header_files/admin_cred.h"
// // #include "../struct_files/customer.h"
// if (sodium_init() < 0) {
//         // Initialization failed
//                  printf("Failed to initialize libsodium.\n");
//                   return -1;
//                 }   

// int save_hashed_password(const char *hashed_password) {
//      char buff[30];
//      int fd = open("/home/aayushi312000/Desktop/Documents/cprograms/mini_project'", O_WRONLY);
//     if (fd == -1) {
//         perror("Failed to open file for writing");
//         return -1;
//     }

//       int wb= write(fd,hashed_password,strlen(hashed_password));
//       if(wb==-1){
//         perror("Failed to write hashed password in the file");
//         return -1;
//       }
      
//       close(fd);
//     return 0;
// }

// int hash_and_store_admin_password(const char *password) {
//     char hashed_password[crypto_pwhash_STRBYTES]; // Buffer to hold the hashed password

//     // Hash the password using Argon2
//     if (crypto_pwhash_str(
//             hashed_password,           // Buffer to store the hashed password
//             password,                  // The password to hash
//             strlen(password),           // Length of the password
//             crypto_pwhash_OPSLIMIT_INTERACTIVE,  // Computational cost
//             crypto_pwhash_MEMLIMIT_INTERACTIVE   // Memory cost
//         ) != 0) {
//         printf("Error hashing password.\n");
//         return -1;
//     }

//     // Store the hashed password in a file
//     if (save_hashed_password(hashed_password) != 0) {
//         printf("Error saving hashed password.\n");
//         return -1;
//     }

//     printf("Password successfully hashed and stored.\n");
//     return 0;
// }

// int verify_password(const char *entered_password) {
//     char stored_hashed_password[crypto_pwhash_STRBYTES];

//     // Load the hashed password from the file
//     if (load_hashed_password(stored_hashed_password) != 0) {
//         printf("Error loading stored hashed password.\n");
//         return -1;
//     }

//     // Verify the entered password
//     if (crypto_pwhash_str_verify(
//             stored_hashed_password,  // The stored hash
//             entered_password,        // The password entered by the admin
//             strlen(entered_password) // Length of the entered password
//         ) == 0) {
//         printf("Password verified successfully!\n");
//         return 0;  // Password is correct
//     } else {
//         printf("Password verification failed.\n");
//         return -1; // Password is incorrect
//     }
// }


// #endif