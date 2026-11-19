#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "port_utils.h"

// stu.cs.jmu.edu IP address
#define SERVER_IP "134.126.141.221"
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    const char *message = "You ain't nothin' but a hound dog!\n";
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
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("[-] Socket creation failed");
        exit(1);
    }
    // Fill out the sockaddr struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    // Fill the IP address field in the sockaddr struct
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("[-] Invalid address");
        close(client_fd);
        exit(1);
    }
    // Connect to the server
    printf("[*] Connecting to %s:%d...\n", SERVER_IP, port);
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("[-] Connection failed");
        close(client_fd);
        exit(1);
    }
    printf("[+] Connected successfully.\n");

    // Write message to the socket
    printf("[<-] Sending: %s", message);
    if (write(client_fd, message, strlen(message)) < 0) {
        perror("[-] Write failed");
        close(client_fd);
        exit(1);
    }

    // Read the response from the socket
    memset(buffer, 0, BUFFER_SIZE);
    int n = read(client_fd, buffer, BUFFER_SIZE - 1);
    if (n > 0) {
        printf("[->] Received from server: %s", buffer);
    }

    close(client_fd);
    return 0;
}
