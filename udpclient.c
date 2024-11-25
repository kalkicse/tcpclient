#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
 
#define SERVER_IP "127.0.0.1"  // Change this to the server's IP address if necessary
#define PORT 12345
#define BUFFER_SIZE 1024
 
int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
 
    // Create a UDP socket (SOCK_DGRAM)
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        exit(1);
    }
 
    // Set up the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
 
    // Send and receive data
    while (1) {
        printf("Enter a message to send (or 'quit' to exit): ");
        fgets(buffer, sizeof(buffer), stdin);
 
        if (strcmp(buffer, "quit\n") == 0) {
            break;
        }
 
        // Send the message to the server
        sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
 
        // Receive the echoed message from the server
        int bytes_received = recvfrom(client_socket, buffer, sizeof(buffer), 0, NULL, NULL);
        if (bytes_received <= 0) {
            perror("Error receiving data");
            break;
        }
 
        buffer[bytes_received] = '\0';  // Null-terminate the received data
        printf("Received: %s", buffer);
    }
 
    // Close the socket
    close(client_socket);
 
    return 0;
}