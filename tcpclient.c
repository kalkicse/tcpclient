#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
 
#define SERVER_IP "127.0.0.1"  // Change this to the actual server IP if needed
#define PORT 12345
#define BUFFER_SIZE 1024
 
int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
 
    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
 
    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
 
    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
 
    printf("Connected to server at %s:%d\n", SERVER_IP, PORT);
 
    // Send and receive data
    while (1) {
        printf("Enter message to send (or 'quit' to exit): ");
        fgets(buffer, sizeof(buffer), stdin);
 
        if (strcmp(buffer, "quit\n") == 0) {
            break;
        }
 
        // Send message to server
        send(client_socket, buffer, strlen(buffer), 0);
 
        // Receive echoed message from server
        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            printf("Connection closed by server.\n");
            break;
        }
 
        buffer[bytes_received] = '\0';  // Null-terminate the received data
        printf("Echoed from server: %s", buffer);
    }
 
    // Close the socket
    close(client_socket);
 
    return 0;
}