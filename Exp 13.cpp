#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

#define MAX_TEXT_LENGTH 1000

// Function to calculate SHA-512 hash
void calculateSHA512(const char *text, unsigned char *hash) {
    SHA512_CTX context;
    SHA512_Init(&context);
    SHA512_Update(&context, text, strlen(text));
    SHA512_Final(hash, &context);
}

int main() {
    char text[MAX_TEXT_LENGTH];
    printf("Enter the text to calculate SHA-512 hash: ");
    fgets(text, sizeof(text), stdin);

    // Remove newline character if present
    size_t len = strlen(text);
    if (text[len - 1] == '\n') {
        text[len - 1] = '\0';
    }

    unsigned char hash[SHA512_DIGEST_LENGTH];
    calculateSHA512(text, hash);

    printf("\nSHA-512 Hash of \"%s\" is:\n", text);
    for (int i = 0; i < SHA512_DIGEST_LENGTH; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");

    return 0;
}
