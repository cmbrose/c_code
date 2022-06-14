#pragma once
#include "ppm.h"
#include "shapes.h"

unsigned short** make_maze
(
    short height, 
    short width, 
    int weight_variance, 
    unsigned short &max_dist,
    bool wrap_maze
);

unsigned short** make_maze_in_shape
(
    short height, 
    short width, 
    int weight_variance, 
    unsigned short &max_dist,
    bool wrap_maze,
	shape s
);

unsigned short** make_maze_in_image
(
    const char *image_path,
    int weight_variance, 
    unsigned short &max_dist,
    bool wrap_maze,
    int *out_width,
    int *out_height
);