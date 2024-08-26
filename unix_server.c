#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>

#define SOCKET_PATH "tpf_unix_sock.server"
#define BUFFER_SIZE 256

int main(void) {
    int server_socket;
    struct sockaddr_un server_sockaddr, client_sockaddr;
    socklen_t client_addr_len = sizeof(struct sockaddr_un);
    char buffer[BUFFER_SIZE];

    // Create socket
    if ((server_socket = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Remove any existing socket file
    unlink(SOCKET_PATH);

    // Set up server address
    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    server_sockaddr.sun_family = AF_UNIX;
    strcpy(server_sockaddr.sun_path, SOCKET_PATH);

    // Bind socket
    if (bind(server_socket, (struct sockaddr*)&server_sockaddr, sizeof(struct sockaddr_un)) == -1) {
        perror("bind");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server is ready to receive messages...\n");

    // Initialize buffer
    memset(buffer, 0, BUFFER_SIZE);

    // Receive message from client
    if (recvfrom(server_socket, buffer, BUFFER_SIZE - 1, 0, 
                 (struct sockaddr*)&client_sockaddr, &client_addr_len) == -1) {
        perror("recvfrom");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    buffer[BUFFER_SIZE - 1] = '\0'; // Ensure null-terminated string
    printf("Received message: %s\n", buffer);

    // Echo the message back to the client
    if (sendto(server_socket, buffer, strlen(buffer), 0, 
               (struct sockaddr*)&client_sockaddr, client_addr_len) == -1) {
        perror("sendto");
    }

    // Close socket and clean up
    close(server_socket);
    unlink(SOCKET_PATH);

    return 0;
}

