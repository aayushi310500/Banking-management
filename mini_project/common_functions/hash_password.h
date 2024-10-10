#ifndef ADMIN_PASSWORD
#define ADMIN_PASSWORD


#include <openssl/evp.h>
#include <openssl/sha.h>  

void hash_password(const char *password, unsigned char *output_hash) {
    EVP_MD_CTX *mdctx;
    const EVP_MD *md = EVP_sha256();
    unsigned int output_len;

    mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, password, strlen(password));
    EVP_DigestFinal_ex(mdctx, output_hash, &output_len);
    EVP_MD_CTX_free(mdctx);
}

// void hash_password(const char *password, unsigned char *output_hash) {
//     // Create a SHA256 context and calculate the hash
//     SHA256_CTX sha256;
//     SHA256_Init(&sha256);                         // Initialize SHA-256 context
//     SHA256_Update(&sha256, password, strlen(password)); // Hash the password
//     SHA256_Final(output_hash, &sha256);           // Store the resulting hash in output_hash
// }
#endif