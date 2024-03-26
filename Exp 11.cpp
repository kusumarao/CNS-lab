#include <stdio.h>

// Function to calculate GCD using Euclidean Algorithm
int gcd(int a, int b) {
    int temp;
    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to calculate modular inverse using Extended Euclidean Algorithm
int mod_inverse(int a, int m) {
    int m0 = m, t, q;
    int x0 = 0, x1 = 1;

    if (m == 1)
        return 0;

    // Apply extended Euclid Algorithm
    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    // Make sure x1 is positive
    if (x1 < 0)
        x1 += m0;

    return x1;
}

int main() {
    // Choose two prime numbers
    int p = 61;
    int q = 53;

    // Calculate n
    int n = p * q;

    // Calculate Euler's Totient Function
    int phi = (p - 1) * (q - 1);

    // Choose an integer e such that 1 < e < phi and gcd(e, phi) = 1
    int e = 17;

    // Calculate the modular inverse of e
    int d = mod_inverse(e, phi);

    // Display public and private keys
    printf("Public Key (n, e): (%d, %d)\n", n, e);
    printf("Private Key (d): %d\n", d);

    // Message to be signed
    int message = 42;

    // Calculate signature
    int signature = 1;
    for (int i = 0; i < d; i++) {
        signature = (signature * message) % n;
    }

    // Display signature
    printf("Signature: %d\n", signature);

    // Verify signature
    int decrypted_message = 1;
    for (int i = 0; i < e; i++) {
        decrypted_message = (decrypted_message * signature) % n;
    }

    // Display verified message
    printf("Verified Message: %d\n", decrypted_message);

    return 0;
}
