#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "port_utils.h"

#define BUFFER_SIZE 1024

void *handle_client(void *arg) {
    char buffer[BUFFER_SIZE];
    int n;
    // Get client_fd from arg
    int client_fd = (int)arg;
    // Chat loop: Handle the currently connected client
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
    // Read client request
        n = read(client_fd, buffer, BUFFER_SIZE - 1);
        if (n <= 0) {
            break;
        }

        printf("[->] Received from client: %s", buffer);

        // Write back answer
        if (write(client_fd, buffer, n) < 0) {
            perror("[-] Write failed");
            break;
        }
    }

    // Close the current client connection
    printf("[*] Client disconnected. Cleaning up socket.\n");
    close(client_fd);
    pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    int port;

    // Check command line arguments
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [optional_port]\n", argv[0]);
        exit(1);
    } else if (argc == 2) {
        // Override with user provided port
        port = atoi(argv[1]);
    } else {
        // Fetch default port from utility function
        char *port_str = get_port();
        port = atoi(port_str);
    }

    // Create the socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("[-] Socket creation failed");
        exit(1);
    }

    // Allow socket reuse if server is killed
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Fill in the sockaddr struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("[-] Bind failed");
        close(server_fd);
        exit(1);
    }

    // Listen
    if (listen(server_fd, 5) < 0) {
        perror("[-] Listen failed");
        close(server_fd);
        exit(1);
    }
    printf("[*] Server listening on port %d...\n", port);

    // Infinite loop: accept new clients forever
    while (1) {
        printf("[*] Waiting for an incoming client connection...\n");

        addr_size = sizeof(client_addr);
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size);
        if (client_fd < 0) {
            perror("[-] Accept failed");
            continue; // Keep server alive even if a single accept fails
        }

        printf("[+] Connected established with %s:%d\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Give this connection to a child thread
        pthread_t tid;
        pthread_create (&tid, NULL, handle_client, (void *)client_fd);
    }

    // Unreachable code, but good practice
    close(server_fd);
    return 0;
}
