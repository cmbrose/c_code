#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include "types.h"
#include "arraylist.h"

typedef struct 
{
	int x, y;
} point_t;

typedef point_t triangle_t[3];

arraylist_p do_triangulation(arraylist_p points);

#endif // TRIANGULATION_H

