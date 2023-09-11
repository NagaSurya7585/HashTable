#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h> // Include Windows socket header
#include <ws2tcpip.h> // Include Windows socket header
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 5

void* handleClient(void* clientSocket) {
    SOCKET clientSock = *(SOCKET*)clientSocket;
    char buffer[1024];
    ssize_t bytesRead;

    while (1) {
        // Receive data from the client
        bytesRead = recv(clientSock, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            perror("Error receiving data from client");
            break;
        }

        // Process the received data (ping)
        buffer[bytesRead] = '\0';
        if (strcmp(buffer, "ping") == 0) {
            // Send a response (pong)
            send(clientSock, "pong", 4, 0);
        }
    }

    // Close the client socket and exit the thread
    closesocket(clientSock);
    free(clientSocket);
    pthread_exit(NULL);
}

int main() {
    SOCKET serverSock, clientSock;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    pthread_t threadID;

    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("Error initializing Winsock");
        exit(EXIT_FAILURE);
    }

    // Create socket
    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock == INVALID_SOCKET) {
        perror("Error creating socket");
        WSACleanup(); // Cleanup Winsock
        exit(EXIT_FAILURE);
    }

    // Set up server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        perror("Error binding socket");
        closesocket(serverSock);
        WSACleanup(); // Cleanup Winsock
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSock, MAX_CLIENTS) == SOCKET_ERROR) {
        perror("Error listening for connections");
        closesocket(serverSock);
        WSACleanup(); // Cleanup Winsock
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept incoming client connection
        clientSock = accept(serverSock, (struct sockaddr*)&clientAddr, &addrLen);
        if (clientSock == INVALID_SOCKET) {
            perror("Error accepting client connection");
            continue;
        }

        // Handle the client in a new thread
        SOCKET* clientSockPtr = malloc(sizeof(SOCKET));
        *clientSockPtr = clientSock;
        if (pthread_create(&threadID, NULL, handleClient, clientSockPtr) != 0) {
            perror("Error creating thread");
            free(clientSockPtr);
            closesocket(clientSock);
            continue;
        }
    }

    // Close the server socket (never reached in this example)
    closesocket(serverSock);
    WSACleanup(); // Cleanup Winsock

    return 0;
}
