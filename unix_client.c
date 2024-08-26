#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>

#define SERVER_PATH "tpf_unix_sock.server"
#define BUFFER_SIZE 256
#define DATA "Hello from client\n"

int main(void) {
    int client_socket;
    struct sockaddr_un server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t server_addr_len;

    // Create socket
    if ((client_socket = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SERVER_PATH);

    // Set up client address
    memset(&client_addr, 0, sizeof(struct sockaddr_un));
    client_addr.sun_family = AF_UNIX;
    snprintf(client_addr.sun_path, sizeof(client_addr.sun_path), "/tmp/client_socket_%ld", (long)getpid());

    // Bind the client socket to a unique path
    if (bind(client_socket, (struct sockaddr*)&client_addr, sizeof(client_addr)) == -1) {
        perror("Client bind failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Prepare message
    strcpy(buffer, DATA);
    printf("Sending data...\n");

    // Send message
    if (sendto(client_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_un)) == -1) {
        perror("sendto");
        close(client_socket);
        unlink(client_addr.sun_path);  // Remove the temporary socket file
        exit(EXIT_FAILURE);
    }

    printf("Data sent!\n");

    // Receive echoed message (if you expect a response)
    server_addr_len = sizeof(server_addr);
    ssize_t recv_len = recvfrom(client_socket, buffer, BUFFER_SIZE - 1, 0, 
                                (struct sockaddr*)&server_addr, &server_addr_len);
    if (recv_len == -1) {
        perror("Receive failed");
    } else {
        buffer[recv_len] = '\0';
        printf("Received message: %s\n", buffer);
    }

    close(client_socket);
    unlink(client_addr.sun_path);  // Remove the temporary socket file
    return 0;
}

