#ifndef BRAIN_ORGAN_EYES_INNER_EAR_H
#define BRAIN_ORGAN_EYES_INNER_EAR_H

#include "inner_ear.h"
#include "Eyes.h"
#include "unified_memory.h"
#include "pml_logic_loop.h"
#include "graph.h"

// Unified Brain Organ Structure
typedef struct {
    InnerEar *inner_ear;
    Eye *eye;
    UnifiedMemoryAndVoice *memory;
    Graph *knowledge_graph;
    EmotionalGraph *emotional_graph;
} BrainOrgan;

// Initialize Brain Organ
BrainOrgan *init_brain_organ(int eye_width, int eye_height, double cochlea_frequency);

// Run Cognitive Cycle
void run_cognitive_cycle(BrainOrgan *brain, int cycles);

// Cleanup Brain Organ
void cleanup_brain_organ(BrainOrgan *brain);

#endif
