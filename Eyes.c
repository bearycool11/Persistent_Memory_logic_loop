#include "eyes.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Initialize the Eye module
Eye *init_eye(int width, int height) {
    Eye *eye = (Eye *)malloc(sizeof(Eye));
    eye->width = width;
    eye->height = height;
    eye->pixel_matrix = (double **)malloc(width * sizeof(double *));
    for (int i = 0; i < width; i++) {
        eye->pixel_matrix[i] = (double *)malloc(height * sizeof(double));
    }
    eye->last_detected_object = (char *)malloc(256 * sizeof(char));
    printf("[Eyes] Initialized with resolution %dx%d.\n", width, height);
    return eye;
}

// Capture a visual frame
void capture_visual_input(Eye *eye, double **frame) {
    for (int i = 0; i < eye->width; i++) {
        for (int j = 0; j < eye->height; j++) {
            eye->pixel_matrix[i][j] = frame[i][j];
        }
    }
    printf("[Eyes] Visual input captured.\n");
}

// Process the visual data (e.g., apply filters)
void process_visual_data(Eye *eye) {
    detect_edges(eye);
    printf("[Eyes] Visual data processed.\n");
}

// Detect edges in the visual input
void detect_edges(Eye *eye) {
    // Example: Simple edge detection (placeholder logic)
    for (int i = 1; i < eye->width - 1; i++) {
        for (int j = 1; j < eye->height - 1; j++) {
            double dx = fabs(eye->pixel_matrix[i + 1][j] - eye->pixel_matrix[i - 1][j]);
            double dy = fabs(eye->pixel_matrix[i][j + 1] - eye->pixel_matrix[i][j - 1]);
            eye->pixel_matrix[i][j] = sqrt(dx * dx + dy * dy); // Magnitude of gradient
        }
    }
    printf("[Eyes] Edge detection complete.\n");
}

// Recognize objects in the scene
void recognize_objects(Eye *eye, Graph *knowledge_graph) {
    // Placeholder for object detection logic
    snprintf(eye->last_detected_object, 256, "Unknown Object");
    printf("[Eyes] Object recognized: %s\n", eye->last_detected_object);

    // Update the knowledge graph with detected object
    embed_novel_topic(knowledge_graph, NULL, eye->last_detected_object);
}

// Cleanup Eye module
void cleanup_eye(Eye *eye) {
    for (int i = 0; i < eye->width; i++) {
        free(eye->pixel_matrix[i]);
    }
    free(eye->pixel_matrix);
    free(eye->last_detected_object);
    free(eye);
    printf("[Eyes] Cleanup complete.\n");
}
