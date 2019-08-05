#pragma once
#include <vector>

#define P_DIST_SQ(P,Q) (((P).x - (Q).x) * ((P).x - (Q).x) + ((P).y - (Q).y) * ((P).y - (Q).y))

typedef struct
{
	int x;
	int y;
} point;

typedef struct 
{
	point start;
	point end;
} segment;

typedef struct 
{
	std::vector<segment> segments;
} shape;

char point_in_shape(point p, shape s);

void make_equilateral_triangle(int w, int h, shape &s);

void make_rectangle(int w, int h, shape &s);

void make_pentagon(int w, int h, shape &s);