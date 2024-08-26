#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int clientSocket, portNum, nBytes;
    char buffer[1024];
    struct sockaddr_in serverAddr; //Structure to store server address
    socklen_t addr_size;
  
    //Create a UDP socket
    clientSocket = socket(PF_INET, SOCK_DGRAM, 0);
    if (clientSocket < 0) {
        perror("socket creation failed");
        exit(1);
    }
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(4999);  //Server port is 5000 - 1 = 4999
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //Use localhost for testing
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
    addr_size = sizeof serverAddr;

    while(1) {
        printf("Type a sentence to send to server:\n"); //user input
        fgets(buffer, 1024, stdin);
        printf("You typed: %s", buffer);
        
        nBytes = strlen(buffer) + 1; //Calculate length of message including null terminator
        
      //Send the message to the server
        sendto(clientSocket, buffer, nBytes, 0, (struct sockaddr *)&serverAddr, addr_size);
        
        //Check if the user wants to exit by typing "Good bye"
        if (strncmp(buffer, "Good bye", 8) == 0) {
            printf("Exiting...\n");
            break;
        }
    }
    
    close(clientSocket);
    return 0;
}
