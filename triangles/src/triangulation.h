#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include "arraylist.h"

typedef struct 
{
	int x, y;
} point_t;

typedef point_t triangle_t[3];

arraylist_p do_triangulation(arraylist_p points, int width, int height);

char point_in_triangle(point_t p, triangle_t tri);

#endif // TRIANGULATION_H

