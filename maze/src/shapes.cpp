#include "shapes.h"
#include "debug.h"

#define CROSS(U,V) ((U).x * (V).y - (U).y * (V).x)
#define DOT(U,V) ((U).x * (V).x + (U).y * (V).y)
#define SEGMENT_VECTOR(S) (VECTOR((S).end, (S).start))
#define VECTOR(S,T) ((point){(S).x - (T).x, (S).y - (T).y})
#define POINT_EQ(P,Q) ((P).x == (Q).x && (P).y == (Q).y)

char range_intersects(float min1, float max1, float min2, float max2)
{
	if (max1 < min2)
	{
		return 0;
	}
	
	if (min1 > max2)
	{
		return 0;
	}
	
	return 1;
}

//https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
char segments_intersect(segment s1, segment s2)
{

	point p = s1.start;
	point q = s2.start;
	
	point r = SEGMENT_VECTOR(s1);
	point s = SEGMENT_VECTOR(s2);
	
	// t = (q − p) x s / (r x s)
	// u = (q − p) x r / (r x s)
	
	point q_minus_p = VECTOR(q, p);
	
	int denom = CROSS(r, s);
	int t_numer = CROSS(q_minus_p, s);
	int u_numer = CROSS(q_minus_p, r);
		
	// case 1: collinear
	if (denom == 0 && u_numer == 0)
	{
		float t0 = 1.0 * DOT(q_minus_p, r) / DOT(r, r);
		float t1 = t0 + 1.0 * DOT(s, r) / DOT(r, r);
				
		if (DOT(s, r) > 0)
		{
			return range_intersects(t0, t1, 0, 1);
		}
		else
		{
			return range_intersects(t1, t0, 0, 1);
		}
	}
	
	// case 2: parallel
	if (denom == 0 && u_numer != 0)
	{
		return 0;
	}
	
	// case 3: normal intersection
	if (denom != 0)
	{	
		float t = 1.0 * t_numer / denom;
		float u = 1.0 * u_numer / denom;
		
		return 0 <= t && t < 1 && 0 <= u && u < 1;
	}		
	
	// case 4: no intersection
	return 0;
}

char point_in_shape(point p, shape s)
{
	point outside = {-1, -1};
	
	segment to_point = {outside, p};
	
	int intersections = 0;
	
	for (unsigned int i = 0; i < s.segments.size(); ++i)
	{	
		segment seg = s.segments[i];
		
		if (POINT_EQ(seg.start, p) || POINT_EQ(seg.end, p))
		{
			return 1;
		}

		if (segments_intersect(to_point, seg))
		{
			++intersections;
		}
	}
	
	return (intersections & 1) == 1;
}

void make_equilateral_triangle(int w, int h, shape &s)
{
	s.segments = std::vector<segment>();
	
	point top = {w / 2, 0};
	point bottom_left = {0, h};
	point bottom_right = {w, h};
	
	/*
		    *
		   ^ \
		  /   \  
		 /     v
		*<------*
	*/
	
	s.segments.push_back({top, bottom_right});
	s.segments.push_back({bottom_right, bottom_left});
	s.segments.push_back({bottom_left, top});
}

void make_rectangle(int w, int h, shape &s)
{
	s = shape();
	s.segments = std::vector<segment>();
	
	point top_left = {0, 0};
	point top_right = {w, 0};
	point bottom_left = {0, h};
	point bottom_right = {w, h};
	
	/*
		*---->*
		^     |
		|     |
		|     v
		*<----*
	*/
	
	s.segments.push_back({top_left, top_right});
	s.segments.push_back({top_right, bottom_right});
	s.segments.push_back({bottom_right, bottom_left});
	s.segments.push_back({bottom_left, top_left});
}

void make_pentagon(int w, int h, shape &s)
{
	s.segments = std::vector<segment>();
	
	point top =          { (int) (0.5000 * w), (int) (0.0000 * h)};
	point mid_left =     { (int) (0.0245 * w), (int) (0.3455 * h) };
	point mid_right =    { (int) (0.9755 * w), (int) (0.3455 * h) };
	point bottom_left =  { (int) (0.2061 * w), (int) (0.9045 * h) };
	point bottom_right = { (int) (0.7939 * w), (int) (0.9045 * h) };
	
	s.segments.push_back({top, mid_right});
	s.segments.push_back({mid_right, bottom_right});
	s.segments.push_back({bottom_right, bottom_left});
	s.segments.push_back({bottom_left, mid_left});
	s.segments.push_back({mid_left, top});
}