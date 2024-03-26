#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define BLOCK_SIZE 64

// Rotate Left Operation
#define ROTLEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

// SHA-1 Functions
#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTLEFT(x, 5) ^ ROTLEFT(x, 14) ^ ROTLEFT(x, 30)))
#define EP1(x) (ROTLEFT(x, 3) ^ ROTLEFT(x, 12) ^ ROTLEFT(x, 22)))
#define EP2(x) (ROTLEFT(x, 1) ^ ROTLEFT(x, 8) ^ ((x) >> 7))
#define EP3(x) (ROTLEFT(x, 19) ^ ROTLEFT(x, 29) ^ ((x) >> 6))

// SHA-1 Constants
uint32_t K[] = {
    0x5A827999,
    0x6ED9EBA1,
    0x8F1BBCDC,
    0xCA62C1D6
};

// SHA-1 Initial Hash Values
uint32_t H[] = {
    0x67452301,
    0xEFCDAB89,
    0x98BADCFE,
    0x10325476,
    0xC3D2E1F0
};

// Padding function
void padMessage(uint8_t *message, uint64_t len) {
    message[len++] = 0x80; // Append a '1' bit

    // Append '0' bits until the message length is 64 bits less than a multiple of 512
    while (len % BLOCK_SIZE != (BLOCK_SIZE - 8)) {
        message[len++] = 0x00;
    }

    // Append the length of the original message in bits as a 64-bit integer
    uint64_t bitLen = len * 8;
    for (int i = 0; i < 8; ++i) {
        message[len++] = (uint8_t)(bitLen >> (56 - i * 8));
    }
}

// Process the message in 512-bit blocks
void processBlocks(uint8_t *message, uint32_t *h) {
    uint32_t w[80];
    uint32_t a, b, c, d, e, temp;

    for (int i = 0; i < 16; ++i) {
        w[i] = (message[i * 4] << 24) |
               (message[i * 4 + 1] << 16) |
               (message[i * 4 + 2] << 8) |
               (message[i * 4 + 3]);
    }

    for (int i = 16; i < 80; ++i) {
        w[i] = ROTLEFT((w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16]), 1);
    }

    a = h[0];
    b = h[1];
    c = h[2];
    d = h[3];
    e = h[4];

    for (int i = 0; i < 80; ++i) {
        if (i < 20) {
            temp = EP0(a) + CH(b, c, d) + e + K[0] + w[i];
        } else if (i < 40) {
            temp = EP1(a) + MAJ(b, c, d) + e + K[1] + w[i];
        } else if (i < 60) {
            temp = EP2(a) + CH(b, c, d) + e + K[2] + w[i];
        } else {
            temp = EP3(a) + MAJ(b, c, d) + e + K[3] + w[i];
        }

        e = d;
        d = c;
        c = ROTLEFT(b, 30);
        b = a;
        a = temp;
    }

    h[0] += a;
    h[1] += b;
    h[2] += c;
    h[3] += d;
    h[4] += e;
}

// Main SHA-1 hashing function
void sha1(uint8_t *message, uint64_t len, uint32_t *digest) {
    uint8_t paddedMessage[(len + 64) + ((64 - (len % BLOCK_SIZE)) % BLOCK_SIZE)];
    memcpy(paddedMessage, message, len);

    padMessage(paddedMessage, len);

    for (uint64_t i = 0; i < len + 64; i += BLOCK_SIZE) {
        processBlocks(paddedMessage + i, digest);
    }
}

// Convert the digest to a readable hex string
void digestToString(uint32_t *digest, char *output) {
    for (int i = 0; i < 5; ++i) {
        sprintf(output + i * 8, "%08x", digest[i]);
    }
}

int main() {
    char input[] = "The quick brown fox jumps over the lazy dog";
    uint32_t digest[5];
    char output[41]; // 40 characters for the digest and 1 for the null terminator

    sha1((uint8_t *)input, strlen(input), digest);
    digestToString(digest, output);

    printf("SHA-1 Digest: %s\n", output);

    return 0;
}
