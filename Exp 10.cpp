#include <stdio.h>
#include <string.h>
#include <mbedtls/md5.h>

void print_hash(unsigned char hash[16]) {
    for (int i = 0; i < 16; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
}

void compute_md5(const char *input) {
    unsigned char output[16]; // MD5 output is 16 bytes
    mbedtls_md5_context ctx;

    mbedtls_md5_init(&ctx);
    mbedtls_md5_starts_ret(&ctx);
    mbedtls_md5_update_ret(&ctx, (const unsigned char*) input, strlen(input));
    mbedtls_md5_finish_ret(&ctx, output);
    mbedtls_md5_free(&ctx);

    print_hash(output);
}

int main() {
    char *input = "Hello, World!";
    printf("Original string: %s\n", input);

    printf("MD5 Hash: ");
    compute_md5(input);

    return 0;
}