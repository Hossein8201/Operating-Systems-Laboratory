#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

void handle_server_response(int sock) {
    char buffer[BUFFER_SIZE];
    int valread = read(sock, buffer, BUFFER_SIZE);
    if (valread > 0) {
        buffer[valread] = '\0';
        printf("%s\n", buffer);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s [server-host-name] [server-port-number] [client-name]\n", argv[0]);
        return 1;
    }

    char *server_host = argv[1];
    int port = atoi(argv[2]);
    char *client_name = argv[3];
    
    int sock;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, server_host, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address or Address not supported");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    // Send client name to server
    send(sock, client_name, strlen(client_name), 0);
    printf("Connected to server as %s\n", client_name);

    char command[BUFFER_SIZE];

    while (1) {
        printf("Enter command: ");
        fgets(command, BUFFER_SIZE, stdin);

        command[strcspn(command, "\n")] = 0;

        send(sock, command, strlen(command), 0);

        if (strncmp(command, "quit", 4) == 0) {
            printf("Exiting...\n");
            break;
        }

        // Receive and print server response
        handle_server_response(sock);
    }

    close(sock);
    return 0;
}