#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
 
#define PORT 12345
#define BUFFER_SIZE 1024
 
int main() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
 
    // Create a UDP socket (SOCK_DGRAM)
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(1);
    }
 
    // Set up the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
 
    // Bind the socket to the server address
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding socket");
        close(server_socket);
        exit(1);
    }
 
    printf("Server listening on port %d...\n", PORT);
 
    // Receive and echo data
    while (1) {
        int bytes_received = recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_addr_len);
        if (bytes_received <= 0) {
            perror("Error receiving data");
            break;
        }
        buffer[bytes_received] = '\0';  // Null-terminate the received data
        printf("Received: %s", buffer);
 
        // Echo the received data back to the client
        sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, client_addr_len);
    }
 
    // Close the socket
    close(server_socket);
 
    return 0;
}