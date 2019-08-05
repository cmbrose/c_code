#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include "triangulation.h"
#include "hashtable.h"
#include "hashtable_iterator.h"

#define NORM(P) ((P).x * (P).x + (P).y * (P).y)
#define CROSS(P, Q) ((P).x * (Q).y - (P).y * (Q).x)
#define POINT_SUB(P, Q) ((point_t) {(P).x - (Q).x, (P).y - (Q).y})
#define DIST(P, Q) sqrt(((P).x - (Q).x) * ((P).x - (Q).x) + ((P).y - (Q).y) * ((P).y - (Q).y))
#define max(X, Y) ((X) > (Y) ? (X) : (Y))
#define min(X, Y) ((X) < (Y) ? (X) : (Y))
#define signof(X) ((X) < 0 ? -1 : 1)

void print_point(point_t p, int newline)
{
    printf("(%d, %d) ", p.x, p.y);
    if (newline) printf("\n");
}

void print_edge(point_t *e, int newline)
{
    print_point(e[0], 0);
    print_point(e[1], newline);
}

void print_triangle(triangle_t t, int newline)
{
    print_point(t[0], 0);
    print_point(t[1], 0);
    print_point(t[2], newline);
}

triangle_t* triangle_make(point_t p0, point_t p1, point_t p2)
{
    triangle_t* ret = (triangle_t*) malloc(sizeof(triangle_t));
    (*ret)[0] = p0; (*ret)[1] = p1; (*ret)[2] = p2;

    return ret;
}

int point_equals(point_t p1, point_t p2)
{
    return (p1.x == p2.x) && (p1.y == p2.y);
}

int edge_hash(void* o)
{
    point_t *edge = o;
    point_t p1 = edge[0], p2 = edge[1];

    int x_min = min(p1.x, p2.x),
        x_max = max(p1.x, p2.x),
        y_min = min(p1.y, p2.y),
        y_max = max(p1.y, p2.y);

    return x_min + 7 * (y_min + 7 * (x_max + 7 * (y_max)));
}

char edge_equals(void* o1, void* o2)
{
    point_t *e1 = o1, *e2 = o2;

    return (point_equals(e1[0], e2[0]) && point_equals(e1[1], e2[1])) ||
           (point_equals(e1[0], e2[1]) && point_equals(e1[1], e2[0]));
}

triangle_t* get_super_triangle(arraylist_p points)
{
    int i;
    int x_min = INT_MAX, x_max = 0;
    int y_min = INT_MAX, y_max = 0;
    int d_x, d_y, d_max;
    int mid_x, mid_y;

    for (i = 0; i < points->size; ++i)
    {
        point_t *p = arraylist_get(points, i);

        x_min = min(x_min, p->x);
        x_max = max(x_max, p->x);
        y_min = min(y_min, p->y);
        y_max = max(y_max, p->y);
    }

    d_x = x_max - x_min;
    d_y = y_max - y_min;
    d_max = max(d_x, d_y);

    mid_x = (x_max + x_min) / 2.0 + 0.5;
    mid_y = (y_max + y_min) / 2.0 + 0.5;

    return triangle_make
    (
        (point_t) {mid_x - 20 * d_max, mid_y - d_max},
        (point_t) {mid_x, mid_y + 20 * d_max},
        (point_t) {mid_x + 20 * d_max, mid_y - d_max}
    );
}

int index_in_triangle(point_t p, triangle_t t)
{
    int i;

    for ( i = 0; i < 3; ++i )
        if ( point_equals(p, t[i]) )
            return i;

    return -1;
}

char point_in_circumcircle(point_t p, triangle_t t)
{
    float m1, m2, b1, b2;
    float mid_x, mid_y;
    int m1_set = 0, m2_set = 0;

    // We're getting perpendicular lines, make sure they aren't vertical
    if (t[0].y != t[1].y) 
    {
        m1 = -1 / (1.0f * (t[0].y - t[1].y) / (t[0].x - t[1].x));

        mid_x = (t[0].x + t[1].x) / 2.0f;
        mid_y = (t[0].y + t[1].y) / 2.0f;

        b1 = mid_y - m1 * mid_x;

        m1_set = 1;
    }
    
    if (t[1].y != t[2].y)
    {
        mid_x = (t[1].x + t[2].x) / 2.0f;
        mid_y = (t[1].y + t[2].y) / 2.0f;

        if (!m1_set) 
        {
            m1 = -1 / (1.0f * (t[1].y - t[2].y) / (t[1].x - t[2].x));
            b1 = mid_y - m1 * mid_x;
        }
        else
        {
            m2 = -1 / (1.0f * (t[1].y - t[2].y) / (t[1].x - t[2].x));
            b2 = mid_y - m2 * mid_x;

            m2_set = 1;
        }
    }

    if (!m2_set)
    {
        m2 = -1 / (1.0f * (t[0].y - t[2].y) / (t[0].x - t[2].x));

        mid_x = (t[0].x + t[2].x) / 2.0f;
        mid_y = (t[0].y + t[2].y) / 2.0f;

        b2 = mid_y - m2 * mid_x;
    }

    float center_x = (b1 - b2) / (m2 - m1);
    float center_y = m1 * center_x + b1;

    float r2 = (t[0].x - center_x) * (t[0].x - center_x) + (t[0].y - center_y) * (t[0].y - center_y);    

    float dist2 = (p.x - center_x) * (p.x - center_x) + (p.y - center_y) * (p.y - center_y);

    return dist2 < r2;
}

arraylist_p do_triangulation(arraylist_p points, int width, int height)
{
    /* 
        This is an implementation of the algorithm described here:
        http://www.codeguru.com/cpp/cpp/algorithms/general/article.php/c8901/Delaunay-Triangles.htm
    */

    int i, j, k;

    arraylist_p tris = arraylist_make(NULL);

    point_t tl = {0, 0},
            bl = {0, height - 1},
            tr = {width - 1, 0},
            br = {width - 1, height - 1};

    triangle_t *t1 = triangle_make(tl, bl, br),
               *t2 = triangle_make(tl, tr, br);

    arraylist_append(tris, t1);
    arraylist_append(tris, t2);

    hashtable_p edge_buff = hashtable_make(5, &edge_hash, &edge_equals, &free);;
    iterator_p edge_iter;

    for (i = 0; i < points->size; ++i)
    {
        point_t *p = arraylist_get(points, i);

        for (j = 0; j < tris->size; ++j)
        {
            triangle_t *t = arraylist_get(tris, j);

            if (point_in_circumcircle(*p, *t))
            {
                for ( k = 0; k < 3; ++k )
                {
                    point_t *edge = malloc(2 * sizeof(point_t));
                    edge[0] = (*t)[k];
                    edge[1] = (*t)[(k + 1) % 3];

                    // The edge existed already, remove it completely
                    point_t *old = hashtable_add(edge_buff, edge);
                    if (old)
                    {
                        hashtable_remove(edge_buff, edge);
                        free(old);
                        free(edge);
                    }
                }

                arraylist_remove(tris, j--);
                free(t);
            }
        }

        edge_iter = hashtable_iter_make(edge_buff);
        while (iterator_has_next(edge_iter))
        {
            point_t *edge = iterator_next(edge_iter);
            arraylist_append(tris, triangle_make(*p, edge[0], edge[1]));
        }

        iterator_destroy(edge_iter);
        hashtable_clear(edge_buff);
    }

    hashtable_destroy(edge_buff);

    return tris;
}

// char point_in_circumcircle(point_t p, triangle_t t)
// {
//     int x0 = t[0].x;
//     int y0 = t[0].y;

//     int x1 = t[1].x;
//     int y1 = t[1].y;

//     int x2 = t[2].x;
//     int y2 = t[2].y;

//     int y10 = y1 - y0;
//     int y21 = y2 - y1;

//     float center_x, center_y;

//     if (y10 == 0)
//     {
//         if (y21 == 0)    // All three vertices are on one horizontal line.
//         {
//             if (x1 > x0)
//             {
//                 if (x2 > x1) x1 = x2;
//             }
//             else
//             {
//                 if (x2 < x0) x0 = x2;
//             }
//             center_x = (x0 + x1) * .5F;
//             center_y = y0;
//         }
//         else    // t[0] and t[1] are on one horizontal line.
//         {
//             float m1 = -1.0 * (x2 - x1) / y21;

//             float mx1 = 1.0 * (x1 + x2) * .5F;
//             float my1 = 1.0 * (y1 + y2) * .5F;

//             center_x = (x0 + x1) * .5F;
//             center_y = m1 * (center_x - mx1) + my1;
//         }
//     }
//     else if (y21 == 0)   // t[1] and t[2] are on one horizontal line.
//     {
//         float m0 = -1.0 * (x1 - x0) / y10;

//         float mx0 = 1.0 * (x0 + x1) * .5F;
//         float my0 = 1.0 * (y0 + y1) * .5F;

//         center_x = (x1 + x2) * .5F;
//         center_y = m0 * (center_x - mx0) + my0;
//     }
//     else    // 'Common' cases, no multiple vertices are on one horizontal line.
//     {
//         float m0 = -1.0 * (x1 - x0) / y10;
//         float m1 = -1.0 * (x2 - x1) / y21;

//         float mx0 = 1.0 *(x0 + x1) * .5F;
//         float my0 = 1.0 *(y0 + y1) * .5F;

//         float mx1 = 1.0 *(x1 + x2) * .5F;
//         float my1 = 1.0 *(y1 + y2) * .5F;

//         center_x = (m0 * mx0 - m1 * mx1 + my1 - my0) / (m0 - m1);
//         center_y = m0 * (center_x - mx0) + my0;
//     }

//     float dx = x0 - center_x;
//     float dy = y0 - center_y;

//     float r2 = dx * dx + dy * dy;   // the radius of the circumcircle, squared

//     float dist2 = (p.x - center_x) * (p.x - center_x) + (p.y - center_y) * (p.y - center_y);

//     return dist2 < r2;
// }

int dot_product(point_t p1, point_t p2)
{
    return p1.x * p2.x + p1.y * p2.y;
}

char point_in_triangle(point_t p, triangle_t tri)
{
    // Courtesy of http://www.blackpawn.com/texts/pointinpoly/

    point_t a = tri[0],
            b = tri[1],
            c = tri[2];
    
    b = (point_t) { b.x - a.x, b.y - a.y }; // B - A
    c = (point_t) { c.x - a.x, c.y - a.y }; // C - A
    p = (point_t) { p.x - a.x, p.y - a.y }; // p - A

    int d = CROSS(b, c);

    int wa = p.x * (b.y - c.y) + p.y * (c.x - b.x) + d;
    int wb = CROSS(p, c);
    int wc = CROSS(b, p);

    wa *= signof(d);
    wb *= signof(d);
    wc *= signof(d);
    d *= signof(d);

    if (wa < 0 || wb < 0 || wc < 0)
        return 0;

    if (wa > d || wb > d || wc > d)
        return 0;

    return 1;
}