
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <openssl/evp.h>

#include "../header_files/data.h"
#include "../common_functions/hash_password.h"
//#include "../common_functions/admin_pass.h"
#include "../struct_files/admin.h"




void storing_admin_details(){
    char buff[1024];
    struct Admin admin;
    char * login="admin123@gmail.com";
    strcpy(admin.login_id, login); 
    char *pass="admin123";
    
      unsigned char hash_pass[EVP_MAX_MD_SIZE];     // Array to hold the resulting hash
     // char pass[HASH_SIZE + 1];
    // Hash the password
      //snprintf(pass, HASH_SIZE + 1, "%s%d", curr_customer.name, curr_customer.ID);
      hash_password(pass, hash_pass);
      char hex_hash[2 * EVP_MAX_MD_SIZE + 1];  // Each byte -> 2 hex digits + null terminator
         for (int i = 0; i < EVP_MAX_MD_SIZE; i++) {
         sprintf(&hex_hash[i * 2], "%02x", hash_pass[i]);
      }
      
      strcpy(admin.password, hex_hash);  
      int fd=open("/home/aayushi312000/MTech/SS/Banking-management/mini_project/stored_data/admin_password.txt",O_WRONLY | O_CREAT | O_APPEND, 0777);
      if(fd==-1){
        perror("Error while opening file(store admin details)");
        exit(1);
     }

     // Serialize the admin struct (login_id and password)
   
    snprintf(buff, sizeof(buff), "Login ID: %s\nPassword (hashed): %s\n", admin.login_id, admin.password);

     int wb = write(fd, buff, strlen(buff));
    if (wb == -1)
    {
        perror("Error while writing Customer record to file!");
        exit(1);
    }
    printf("data is saved!");
    close(fd);
      //strcpy(curr_customer.password,hash_pass);

    // admin.password="";
}


void main(){
    storing_admin_details();
}