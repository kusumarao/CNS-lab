#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define PORT 4433
#define CERT_FILE "server.crt"
#define KEY_FILE "server.key"

int main() {
    SSL_CTX *ctx;
    SSL *ssl;
    int sockfd, newsockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t sin_size;
    char buffer[1024];
    int bytes;

    // Initialize OpenSSL
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    // Create a new SSL context
    ctx = SSL_CTX_new(SSLv23_server_method());
    if (ctx == NULL) {
        perror("SSL_CTX_new");
        exit(EXIT_FAILURE);
    }

    // Load the server's certificate and key files
    if (SSL_CTX_use_certificate_file(ctx, CERT_FILE, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, KEY_FILE, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    // Create a TCP socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(server_addr.sin_zero), '\0', 8);

    // Bind socket to address
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(sockfd, 10) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while(1) {
        // Accept a new connection
        sin_size = sizeof(struct sockaddr_in);
        if ((newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size)) == -1) {
            perror("accept");
            continue;
        }

        printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Create a new SSL structure for the connection
        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, newsockfd);

        // Perform SSL handshake
        if (SSL_accept(ssl) == -1) {
            ERR_print_errors_fp(stderr);
        } else {
            // Serve a simple HTML page
            const char *html = "<html><head><title>OpenSSL Web Server</title></head><body><h1>Welcome to OpenSSL Web Server!</h1></body></html>";
            sprintf(buffer, "HTTP/1.1 200 OK\r\nContent-Length: %lu\r\nContent-Type: text/html\r\n\r\n%s", strlen(html), html);
            SSL_write(ssl, buffer, strlen(buffer));
        }

        // Close SSL connection
        SSL_shutdown(ssl);
        SSL_free(ssl);

        // Close the new socket
        close(newsockfd);
    }

    // Close the listening socket
    close(sockfd);

    // Free the SSL context
    SSL_CTX_free(ctx);

    return 0;
}
