#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_PRODUCTS 100
#define BUFFER_SIZE 1024

struct Product {
    char name[100];
    int count;
};

struct Product warehouse[MAX_PRODUCTS];
int product_count = 0;

int find_product(char *name) {
    for (int i = 0; i < product_count; i++) {
        if (strcmp(warehouse[i].name, name) == 0) {
            return i;
        }
    }
    return -1; 
}

void list_products(int socket) {
    char response[BUFFER_SIZE] = "";
    for (int i = 0; i < product_count; i++) {
        char line[200];
        sprintf(line, "Product: %s, Count: %d\n", warehouse[i].name, warehouse[i].count);
        strcat(response, line);
    }
    send(socket, response, strlen(response), 0);
}

void create_product(char *name, int count, int socket) {
    if (find_product(name) != -1) {
        char *error = "Error: Product already exists.\n";
        send(socket, error, strlen(error), 0);
        return;
    }

    if (product_count >= MAX_PRODUCTS) {
        char *error = "Error: Warehouse is full.\n";
        send(socket, error, strlen(error), 0);
        return;
    }

    if (count < 0) {
        char *error = "Error: Invalid amount.\n";
        send(socket, error, strlen(error), 0);
        return;
    }
    strcpy(warehouse[product_count].name, name);
    warehouse[product_count].count = count;
    product_count++;

    char *success = "Product created successfully.\n";
    send(socket, success, strlen(success), 0);
}

void add_product(char *name, int amount, int socket) {
    int index = find_product(name);
    if (index == -1) {
        char *error = "Error: Product not found.\n";
        send(socket, error, strlen(error), 0);
        return;
    }

    warehouse[index].count += amount;
    char *success = "Product quantity updated successfully.\n";
    send(socket, success, strlen(success), 0);
}

void reduce_product(char *name, int amount, int socket) {
    int index = find_product(name);
    if (index == -1) {
        char *error = "Error: Product not found.\n";
        send(socket, error, strlen(error), 0);
        return;
    }

    if (warehouse[index].count < amount) {
        char *error = "Error: Not enough stock to reduce.\n";
        send(socket, error, strlen(error), 0);
        return;
    }

    warehouse[index].count -= amount;
    char *success = "Product quantity reduced successfully.\n";
    send(socket, success, strlen(success), 0);
}

void remove_product(char *name, int socket) {
    int index = find_product(name);
    if (index == -1) {
        char *error = "Error: Product not found.\n";
        send(socket, error, strlen(error), 0);
        return;
    }

    if (warehouse[index].count != 0) {
        char *error = "Error: Product must have zero quantity to be removed.\n";
        send(socket, error, strlen(error), 0);
        return;
    }

    for (int i = index; i < product_count - 1; i++) {
        warehouse[i] = warehouse[i + 1];
    }
    product_count--;

    char *success = "Product removed successfully.\n";
    send(socket, success, strlen(success), 0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s [port]\n", argv[0]);
        return 1;
    }

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    int port = atoi(argv[1]);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", port);

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    char command[BUFFER_SIZE];
    char arg1[BUFFER_SIZE];
    char arg2[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        read(new_socket, buffer, BUFFER_SIZE);
        printf("Received command: %s\n", buffer);

        memset(arg1, 0, BUFFER_SIZE);
        memset(arg2, 0, BUFFER_SIZE);
        memset(command, 0, BUFFER_SIZE);
        sscanf(buffer, "%s %s %s", command, arg1, arg2);
        printf("%s: %s %s %s\n", buffer, command, arg1, arg2);
        // printf("%s\n", command);
        if (strcmp(command, "list") == 0) {
            list_products(new_socket);
        } else if (strcmp(command, "create") == 0) {
            int count = (arg2[0] != '\0') ? atoi(arg2) : 0;
            create_product(arg1, count, new_socket);
        } else if (strcmp(command, "add") == 0) {
            add_product(arg1, atoi(arg2), new_socket);
        } else if (strcmp(command, "reduce") == 0) {
            reduce_product(arg1, atoi(arg2), new_socket);
        } else if (strcmp(command, "remove") == 0) {
            remove_product(arg1, new_socket);
        } else if (strcmp(command, "quit") == 0) {
            close(new_socket);
            break;
        } else {
            char *error = "Error: Invalid command.\n";
            send(new_socket, error, strlen(error), 0);
        }
    }

    close(server_fd);
    return 0;
}