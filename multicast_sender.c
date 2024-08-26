#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error(const char *msg) {
    perror(msg); //print error message
    exit(1);
}

int main(int argc, char *argv[]) {
    //check number of arguments
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <Multicast IP> <Interface IP> <Multicast Port>\n", argv[0]);
        exit(1);
    }

    const char *multicast_ip = argv[1];
    const char *interface_ip = argv[2];
    int multicast_port = atoi(argv[3]);
    int unicast_port = multicast_port - 1;

    int unicast_sock, multicast_sock;
    struct sockaddr_in unicast_addr, multicast_addr, client_addr;
    struct in_addr local_interface;
    char buffer[1024];
    socklen_t addr_size;

    //Create UDP socket for unicast communication
    unicast_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (unicast_sock < 0) {
        error("unicast socket()");
    }

    //Set up the address structure for the unicast socket
    memset(&unicast_addr, 0, sizeof(unicast_addr));
    unicast_addr.sin_family = AF_INET;
    unicast_addr.sin_addr.s_addr = htonl(INADDR_ANY);  //Bind to any local IP address
    unicast_addr.sin_port = htons(unicast_port); //bind to unicast port

    //Bind the unicast socket to the specified address and port
    if (bind(unicast_sock, (struct sockaddr *)&unicast_addr, sizeof(unicast_addr)) < 0) {
        error("unicast bind()");
    }

    //Create UDP socket for multicast communication
    multicast_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (multicast_sock < 0) {
        error("multicast socket()");
    }

    //Set the local interface for multicast communication
    local_interface.s_addr = inet_addr(interface_ip);
    if (setsockopt(multicast_sock, IPPROTO_IP, IP_MULTICAST_IF, (char *)&local_interface, sizeof(local_interface)) < 0) {
        error("setsockopt()");
    }

    //Set up the address structure for the multicast group
    memset(&multicast_addr, 0, sizeof(multicast_addr));
    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_addr.s_addr = inet_addr(multicast_ip);
    multicast_addr.sin_port = htons(multicast_port);


    addr_size = sizeof(client_addr);
    while (1) { //Receive a message from the unicast client
        int nbytes = recvfrom(unicast_sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&client_addr, &addr_size);
        if (nbytes < 0) {
            error("recvfrom()");
        }
        buffer[nbytes] = '\0'; //Null-terminate the received message
        
        printf("Received msg from UDP client: %s", buffer);
        printf("\n");
        printf("Sending msg to the multicast receiver(s)\n");

        //Send the received message to the multicast group
        if (sendto(multicast_sock, buffer, strlen(buffer), 0, (struct sockaddr *) &multicast_addr, sizeof(multicast_addr)) < 0) {
            error("sendto()");
        }
    }

    close(unicast_sock);
    close(multicast_sock);
    return 0;
}
