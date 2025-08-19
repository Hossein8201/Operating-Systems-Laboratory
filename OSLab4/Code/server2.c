#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 100
#define MAX_PRODUCTS 100
#define BUFFER_SIZE 1024

struct Product {
    char name[100];
    int count;
};

struct Client {
    char name[100];
    struct Product products[MAX_PRODUCTS];
    int product_count;
    int socket;
};

struct Client clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t client_lock;

int find_client(char *name) {
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

int find_product(struct Client *client, char *name) {
    for (int i = 0; i < client->product_count; i++) {
        if (strcmp(client->products[i].name, name) == 0) {
            return i;
        }
    }
    return -1; 
}

void list_products(int client_index) {
    char response[BUFFER_SIZE] = "";
    struct Client *client = &clients[client_index];

    for (int i = 0; i < client->product_count; i++) {
        char line[200];
        sprintf(line, "Product: %s, Count: %d\n", client->products[i].name, client->products[i].count);
        strcat(response, line);
    }

    send(client->socket, response, strlen(response), 0);
}

void create_product(int client_index, char *name, int count) {
    struct Client *client = &clients[client_index];

    if (find_product(client, name) != -1) {
        char *error = "Error: Product already exists.\n";
        send(client->socket, error, strlen(error), 0);
        return;
    }

    if (client->product_count >= MAX_PRODUCTS) {
        char *error = "Error: Product list is full.\n";
        send(client->socket, error, strlen(error), 0);
        return;
    }

    if (count < 0) {
        char *error = "Error: Invalid amount.\n";
        send(client->socket, error, strlen(error), 0);
        return;
    }

    strcpy(client->products[client->product_count].name, name);
    client->products[client->product_count].count = count;
    client->product_count++;

    char *success = "Product created successfully.\n";
    send(client->socket, success, strlen(success), 0);
}

void add_product(int client_index, char *name, int amount) {
    struct Client *client = &clients[client_index];
    int index = find_product(client, name);

    if (index == -1) {
        char *error = "Error: Product not found.\n";
        send(client->socket, error, strlen(error), 0);
        return;
    }

    client->products[index].count += amount;
    char *success = "Product quantity updated successfully.\n";
    send(client->socket, success, strlen(success), 0);
}

void reduce_product(int client_index, char *name, int amount) {
    struct Client *client = &clients[client_index];
    int index = find_product(client, name);

    if (index == -1) {
        char *error = "Error: Product not found.\n";
        send(client->socket, error, strlen(error), 0);
        return;
    }

    if (client->products[index].count < amount) {
        char *error = "Error: Not enough stock to reduce.\n";
        send(client->socket, error, strlen(error), 0);
        return;
    }

    client->products[index].count -= amount;
    char *success = "Product quantity reduced successfully.\n";
    send(client->socket, success, strlen(success), 0);
}

void remove_product(int client_index, char *name) {
    struct Client *client = &clients[client_index];
    int index = find_product(client, name);

    if (index == -1) {
        char *error = "Error: Product not found.\n";
        send(client->socket, error, strlen(error), 0);
        return;
    }

    if (client->products[index].count != 0) {
        char *error = "Error: Product must have zero quantity to be removed.\n";
        send(client->socket, error, strlen(error), 0);
        return;
    }

    for (int i = index; i < client->product_count - 1; i++) {
        client->products[i] = client->products[i + 1];
    }
    client->product_count--;

    char *success = "Product removed successfully.\n";
    send(client->socket, success, strlen(success), 0);
}

void send_product(int client_index, char *target_name, char *product_name, int amount) {
    struct Client *sender = &clients[client_index];
    int product_index = find_product(sender, product_name);

    if (product_index == -1) {
        char *error = "Error: Product not found.\n";
        send(sender->socket, error, strlen(error), 0);
        return;
    }

    if (sender->products[product_index].count < amount) {
        char *error = "Error: Not enough stock to transfer.\n";
        send(sender->socket, error, strlen(error), 0);
        return;
    }

    if (amount < 0) {
        char *error = "Error: Invalid amount.\n";
        send(sender->socket, error, strlen(error), 0);
        return;
    }

    pthread_mutex_lock(&client_lock);
    int target_index = find_client(target_name);
    if (target_index == -1) {
        char *error = "Error: Target client not found.\n";
        send(sender->socket, error, strlen(error), 0);
        pthread_mutex_unlock(&client_lock);
        return;
    }

    struct Client *receiver = &clients[target_index];
    int receiver_product_index = find_product(receiver, product_name);

    if (receiver_product_index == -1) {
        strcpy(receiver->products[receiver->product_count].name, product_name);
        receiver->products[receiver->product_count].count = amount;
        receiver->product_count++;
    } else {
        receiver->products[receiver_product_index].count += amount;
    }

    sender->products[product_index].count -= amount;
    pthread_mutex_unlock(&client_lock);

    char *success = "Product transferred successfully.\n";
    send(sender->socket, success, strlen(success), 0);
}

void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    char buffer[BUFFER_SIZE] = {0};
    char client_name[100];

    read(client_socket, client_name, sizeof(client_name));
    pthread_mutex_lock(&client_lock);
    strcpy(clients[client_count].name, client_name);
    clients[client_count].socket = client_socket;
    client_count++;
    pthread_mutex_unlock(&client_lock);
    char command[BUFFER_SIZE], arg1[BUFFER_SIZE], arg2[BUFFER_SIZE], arg3[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(client_socket, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            close(client_socket);
            pthread_exit(NULL);
        }

        memset(arg1, 0, BUFFER_SIZE);
        memset(arg2, 0, BUFFER_SIZE);
        memset(arg3, 0, BUFFER_SIZE);
        memset(command, 0, BUFFER_SIZE);

        sscanf(buffer, "%s %s %s %s", command, arg1, arg2, arg3);

        int client_index = find_client(client_name);
        if (client_index == -1) {
            char *error = "Error: Client not found.\n";
            send(client_socket, error, strlen(error), 0);
            continue;
        }

        if (strcmp(command, "list") == 0) {
            list_products(client_index);
        } else if (strcmp(command, "create") == 0) {
            int count = (arg2[0] != '\0') ? atoi(arg2) : 0;
            create_product(client_index, arg1, count);
        } else if (strcmp(command, "add") == 0) {
            add_product(client_index, arg1, atoi(arg2));
        } else if (strcmp(command, "reduce") == 0) {
            reduce_product(client_index, arg1, atoi(arg2));
        } else if (strcmp(command, "remove") == 0) {
            remove_product(client_index, arg1);
        } else if (strcmp(command, "send") == 0) {
            send_product(client_index, arg1, arg2, atoi(arg3));
        } else if (strcmp(command, "quit") == 0) {
            close(client_socket);
            pthread_exit(NULL);
        } else {
            char *error = "Error: Invalid command.\n";
            send(client_socket, error, strlen(error), 0);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s [port]\n", argv[0]);
        return 1;
    }

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int port = atoi(argv[1]);

    // Create server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Bind server to the specified port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", port);

    pthread_mutex_init(&client_lock, NULL);

    while (1) {
        // Accept new connections
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        // Handle each client in a new thread
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void *)&new_socket) != 0) {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }

        pthread_detach(thread_id); // Automatically clean up thread resources after it finishes
    }

    pthread_mutex_destroy(&client_lock);
    return 0;
}