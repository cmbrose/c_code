#pragma once
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