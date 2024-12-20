#ifndef EYES_H
#define EYES_H

#include "unified_memory.h"
#include "graph.h"
#include "inner_ear.h"

typedef struct {
    double **pixel_matrix; // 2D array for image data
    int width;
    int height;
    char *last_detected_object; // Buffer for object names
} Eye;

Eye *init_eye(int width, int height);
void capture_visual_input(Eye *eye, double **frame);
void process_visual_data(Eye *eye);
void detect_edges(Eye *eye);
void recognize_objects(Eye *eye, Graph *knowledge_graph);
void cleanup_eye(Eye *eye);

#endif
