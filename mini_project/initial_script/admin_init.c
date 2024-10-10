#include "../header_files/data.h"
#include "hash_password.h"
#include "admin_pass.h"

void storing_admin_details(){
    char buff[1024];
    struct Admin admin;
    int fd=open("/home/aayushi312000/Desktop/Documents/cprograms/mini_project'/stored_data/admin_password.txt",O_WRONLY);
    if(fd==-1){
        perror("Error while opening file(store admin details)");
        exit(1);
    }
    admin.login_id="admin123@gmail.com";
    char *pass="admin123";

      unsigned char hash_pass[EVP_MAX_MD_SIZE];     // Array to hold the resulting hash
      char pass[HASH_SIZE + 1];
    // Hash the password
      //snprintf(pass, HASH_SIZE + 1, "%s%d", curr_customer.name, curr_customer.ID);
      char hex_hash[2 * EVP_MAX_MD_SIZE + 1];  // Each byte -> 2 hex digits + null terminator
         for (int i = 0; i < EVP_MAX_MD_SIZE; i++) {
    sprintf(&hex_hash[i * 2], "%02x", hash_pass[i]);
      }
       hash_password(pass, hash_pass);
      strcpy(curr_customer.password,hash_pass);

    // admin.password="";
}


void main(){
    storing_admin_details();
}