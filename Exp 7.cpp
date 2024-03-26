#include<stdio.h>
#include<openssl/blowfish.h>
#include<string.h>

#define MAX_LENGTH 1024
#define KEY "my_secret_key"

void encrypt_message(const char* message, unsigned char* encrypted, BF_KEY* key) {
    BF_set_key(key, strlen(KEY), (const unsigned char*)KEY);
    BF_ecb_encrypt((const unsigned char*)message, encrypted, key, BF_ENCRYPT);
}

void decrypt_message(const unsigned char* encrypted, char* decrypted, BF_KEY* key) {
    BF_set_key(key, strlen(KEY), (const unsigned char*)KEY);
    BF_ecb_encrypt(encrypted, (unsigned char*)decrypted, key, BF_DECRYPT);
}

int main() {
    char message[MAX_LENGTH];
    unsigned char encrypted[MAX_LENGTH];
    char decrypted[MAX_LENGTH];

    BF_KEY key;
    memset(&key, 0, sizeof(BF_KEY));

    printf("Enter message to encrypt: ");
    fgets(message, MAX_LENGTH, stdin);
    message[strcspn(message, "\n")] = 0;

    encrypt_message(message, encrypted, &key);
    printf("Encrypted message: ");
    for (int i = 0; i < strlen(message); i++) {
        printf("%02x", encrypted[i]);
    }
    printf("\n");

    decrypt_message(encrypted, decrypted, &key);
    printf("Decrypted message: %s\n", decrypted);

    return 0;
}
