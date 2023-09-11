#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h> // Include Windows socket header
#include <ws2tcpip.h> // Include Windows socket header
#include <unistd.h> // Used for sleep on Windows

#define SERVER_IP "127.0.0.1"
#define PORT 8080

int main() {
    SOCKET clientSock;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("Error initializing Winsock");
        exit(EXIT_FAILURE);
    }

    // Create socket
    clientSock = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSock == INVALID_SOCKET) {
        perror("Error creating socket");
        WSACleanup(); // Cleanup Winsock
        exit(EXIT_FAILURE);
    }

    // Set up server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        perror("Invalid server address");
        closesocket(clientSock);
        WSACleanup(); // Cleanup Winsock
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(clientSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        perror("Error connecting to server");
        closesocket(clientSock);
        WSACleanup(); // Cleanup Winsock
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Send a "ping" message to the server
        send(clientSock, "ping", 4, 0);
        printf("Sent: ping\n");

        // Receive the response from the server
        ssize_t bytesRead = recv(clientSock, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            perror("Error receiving data from server");
            break;
        }

        buffer[bytesRead] = '\0';
        if (strcmp(buffer, "pong") == 0) {
            printf("Received: pong\n");
        }

        Sleep(1000); // Wait for 1 second before sending the next ping (use Sleep for Windows)
    }

    // Close the client socket
    closesocket(clientSock);
    WSACleanup(); // Cleanup Winsock

    return 0;
}
