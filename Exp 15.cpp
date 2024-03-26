#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/evp.h>

#define RSA_KEY_LENGTH 2048
#define CERT_DAYS 365

int main() {
    EVP_PKEY *pkey = NULL;
    X509 *cert = NULL;
    X509_NAME *name = NULL;
    FILE *fp = NULL;

    // Initialize OpenSSL library
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    // Generate RSA key pair
    pkey = EVP_PKEY_new();
    RSA *rsa_key = RSA_generate_key(RSA_KEY_LENGTH, RSA_F4, NULL, NULL);
    EVP_PKEY_assign_RSA(pkey, rsa_key);

    // Create certificate
    cert = X509_new();
    X509_set_version(cert, 2); // X509v3 certificate
    ASN1_INTEGER_set(X509_get_serialNumber(cert), 1); // Serial number
    X509_gmtime_adj(X509_get_notBefore(cert), 0); // Validity start time
    X509_gmtime_adj(X509_get_notAfter(cert), CERT_DAYS * 24 * 3600); // Validity end time
    X509_set_pubkey(cert, pkey);

    // Set subject of the certificate
    name = X509_get_subject_name(cert);
    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char *)"John Doe", -1, -1, 0);

    // Sign the certificate with the private key
    X509_sign(cert, pkey, EVP_sha256());

    // Write private key to file
    fp = fopen("private.pem", "w");
    PEM_write_PrivateKey(fp, pkey, NULL, NULL, 0, NULL, NULL);
    fclose(fp);

    // Write certificate to file
    fp = fopen("certificate.pem", "w");
    PEM_write_X509(fp, cert);
    fclose(fp);

    // Cleanup
    EVP_PKEY_free(pkey);
    X509_free(cert);

    return 0;
}
