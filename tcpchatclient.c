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
 
    // Create a socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        exit(1);
    }
 
    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
 
    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        close(client_socket);
        exit(1);
    }
 
    printf("Connected to server at %s:%d\n", SERVER_IP, PORT);
 
    // Chat loop
    while (1) {
        // Prompt for a message to send
        printf("Client (Type 'quit' to exit): ");
        fgets(buffer, sizeof(buffer), stdin);
 
        // Send the message to the server
        send(client_socket, buffer, strlen(buffer), 0);
 
        // Check if the client wants to quit
        if (strcmp(buffer, "quit\n") == 0) {
            break;
        }
 
        // Receive a message from the server
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            printf("Connection closed by server.\n");
            break;
        }
 
        buffer[bytes_received] = '\0';  // Null-terminate the received data
        printf("Server: %s", buffer);
    }
 
    // Close the socket
    close(client_socket);
 
    return 0;
}