#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "io_socket.h"
#include "pml_logic_loop.h"
#include "memory_silo.h"

// Global IO socket instance
io_socket_t io_socket;

// Function to send knowledge graph data (e.g., metrics)
void send_graph_node(io_socket_t *io_socket, const char *node_name, pml_metrics_t *metrics) {
    if (!io_socket || !node_name || !metrics) {
        fprintf(stderr, "Error: Invalid parameters for sending graph node\n");
        return;
    }

    size_t node_name_length = strlen(node_name) + 1; // Include null terminator
    size_t metrics_size = sizeof(pml_metrics_t);
    size_t total_size = node_name_length + metrics_size;

    // Allocate memory for the data to be sent
    char *data = malloc(total_size);
    if (!data) {
        perror("Memory allocation failed");
        return;
    }

    // Copy the node name and metrics into the data buffer
    memcpy(data, node_name, node_name_length);
    memcpy(data + node_name_length, metrics, metrics_size);

    // Send the data through the socket
    if (io_socket_send(io_socket->socket, data, total_size) < 0) {
        fprintf(stderr, "Failed to send data\n");
    } else {
        printf("Knowledge graph data sent successfully\n");
    }

    free(data);
}

// Function to initialize the PML logic loop
void pml_logic_loop_init_wrapper(io_socket_t *io_socket, int memory_silo_id) {
    if (!io_socket) {
        fprintf(stderr, "Error: IO socket instance is null\n");
        return;
    }
    pml_logic_loop_init(memory_silo_id, io_socket->socket);
}

// Function to handle the main PML logic loop with socket communication
void pml_logic_loop_process_wrapper(io_socket_t *io_socket, memory_silo_t *silo) {
    if (!io_socket || !silo) {
        fprintf(stderr, "Error: Invalid parameters for PML logic loop processing\n");
        return;
    }

    // Assuming the silo contains knowledge graph or state data
    send_graph_node(io_socket, "Model A", &silo->metrics);

    // Process the PML logic loop
    pml_logic_loop_process(io_socket->socket, NULL, 0);
}

// Function to initialize the IO socket
int io_socket_init_wrapper(io_socket_t *io_socket, const char *ip, int port) {
    return io_socket_init(io_socket, ip, port);
}

// Function to clean up and close the IO socket
void io_socket_cleanup(io_socket_t *io_socket) {
    if (io_socket) {
        io_socket_close(io_socket);
        printf("IO socket cleaned up successfully\n");
    }
}

// Function to save the state of the memory silo
void save_memory_silo(const char* filename, memory_silo_t* memory_silo) {
    if (!memory_silo) {
        fprintf(stderr, "Error: Memory silo is null, cannot save\n");
        return;
    }
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Failed to open file for saving Memory Silo");
        return;
    }
    fwrite(memory_silo, sizeof(memory_silo_t), 1, file);
    fclose(file);
    printf("Memory silo state saved to %s\n", filename);
}

// Function to load the state of the memory silo
memory_silo_t* load_memory_silo(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Failed to open file for loading Memory Silo");
        return NULL;
    }

    memory_silo_t* memory_silo = malloc(sizeof(memory_silo_t));
    if (memory_silo == NULL) {
        perror("Memory allocation failed for Memory Silo");
        fclose(file);
        return NULL;
    }
    fread(memory_silo, sizeof(memory_silo_t), 1, file);
    fclose(file);

    printf("Memory silo state loaded from %s\n", filename);
    return memory_silo;
}

// Main function to demonstrate persistence with socket and PML logic loop
int main() {
    // Initialize the IO socket
    if (io_socket_init_wrapper(&io_socket, "127.0.0.1", 8080) < 0) {
        fprintf(stderr, "Failed to initialize IO socket\n");
        return -1;
    }

    // Initialize memory silo and PML logic loop
    memory_silo_t silo = {0}; // Example memory silo
    pml_logic_loop_init_wrapper(&io_socket, silo.id);

    // Load previous state from files if needed
    memory_silo_t* loaded_memory_silo = load_memory_silo("memory_silo_state.dat");
    if (loaded_memory_silo) {
        printf("Loaded memory silo ID: %d\n", loaded_memory_silo->id);
        free(loaded_memory_silo); // Free after use
    }

    // Main loop for processing
    int iterations = 0;
    while (iterations < 5) { // Example limited loop
        printf("Processing iteration %d\n", iterations + 1);
        pml_logic_loop_process_wrapper(&io_socket, &silo);
        iterations++;
    }

    // Save memory silo state before exiting
    save_memory_silo("memory_silo_state.dat", &silo);

    // Clean up and close the socket
    io_socket_cleanup(&io_socket);

    return 0;
}