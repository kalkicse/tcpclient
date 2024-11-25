#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include <arpa/inet.h>
 
#define PORT 12345

#define BUFFER_SIZE 1024
 
int main() {

    int server_socket, client_socket;

    struct sockaddr_in server_addr, client_addr;

    socklen_t client_addr_len = sizeof(client_addr);

    char buffer[BUFFER_SIZE];
 
    // Create socket

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1) {

        perror("Error creating socket");

        exit(EXIT_FAILURE);

    }
 
    // Set up server address

    server_addr.sin_family = AF_INET;

    server_addr.sin_port = htons(PORT);

    server_addr.sin_addr.s_addr = INADDR_ANY;
 
    // Bind socket to address

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {

        perror("Error binding socket");

        close(server_socket);

        exit(EXIT_FAILURE);

    }
 
    // Listen for connections

    if (listen(server_socket, 5) == -1) {

        perror("Error listening for connections");

        close(server_socket);

        exit(EXIT_FAILURE);

    }
 
    printf("Server listening on port %d...\n", PORT);
 
    // Accept client connection

    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);

    if (client_socket == -1) {

        perror("Error accepting connection");

        close(server_socket);

        exit(EXIT_FAILURE);

    }
 
    printf("Client connected.\n");
 
    // Echo loop

    while (1) {

        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received <= 0) {

            printf("Connection closed by client.\n");

            break;

        }
 
        buffer[bytes_received] = '\0';  // Null-terminate the received data

        printf("Received: %s", buffer);
 
        // Echo back the received message

        send(client_socket, buffer, bytes_received, 0);

    }
 
    // Close sockets

    close(client_socket);

    close(server_socket);
 
    return 0;

}
 

 