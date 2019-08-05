#include <stdlib.h>
#include "triangulation.h"
#include "arraylist.h"
#include "ppm.h"
#include "options.h"

#define max(X, Y) ((X) > (Y) ? (X) : (Y))

arraylist_p get_tris(int cnt, ppm_t image);
arraylist_p get_colors(arraylist_p tris, ppm_t image);
void set_colors(arraylist_p colors, arraylist_p tris, ppm_t image);
int right_comp(const void *pa, const void *pb);

void triangles(ppm_t src, ppm_t res, method_options_t opts)
{
	triangles_options_t t_opts = opts.triangles;
	
	int cnt = t_opts.count;
	
    arraylist_p tris = get_tris(cnt, src);

    arraylist_p colors = get_colors(tris, src);

    set_colors(colors, tris, res);

    arraylist_destroy(tris);
    arraylist_destroy(colors);
}

int right_comp(const void *pa, const void *pb)
{
    point_t *a = **((triangle_t**) pa),
            *b = **((triangle_t**) pb);

    int a_max = max(a[0].x, max(a[1].x, a[2].x));
    int b_max = max(b[0].x, max(b[1].x, b[2].x));

    return a_max - b_max;
}

arraylist_p get_tris(int cnt, ppm_t image)
{
    arraylist_p points = arraylist_make(NULL);

    point_t *p;

    int i;
    for (i = 0; i < cnt; ++i)
    {
        p = malloc(sizeof(point_t));
        *p = (point_t) { rand() % image.w, rand() % image.h};
        arraylist_append(points, p);
    }

    arraylist_p tris = do_triangulation(points, image.w, image.h);

    for (i = 0; i < cnt; ++i)
    {
        free(arraylist_get(points, i));
    }

    arraylist_destroy(points);

    qsort(tris->array, tris->size, sizeof(triangle_t*), right_comp);

    return tris;
}

arraylist_p get_colors(arraylist_p tris, ppm_t image)
{
    arraylist_p data = arraylist_make(NULL);

    typedef struct 
    {
        unsigned long long r, g, b, cnt;
    } data_t;

    int i;
    for (i = 0; i < tris->size; ++i) 
    {
        data_t *d = malloc(sizeof(data_t));
        *d = (data_t) {0,0,0,0};

        arraylist_append(data, d);
    }

    int x, y, s = 0;
    for (x = 0; x < image.w; ++x) 
    {
        for (y = 0; y < image.h; ++y)
        {
            while (s < tris->size)
            {
                point_t *t = *((triangle_t*) arraylist_get(tris, s));

                int t_right = max(t[0].x, max(t[1].x, t[2].x));

                if (t_right < x)
                    ++s;
                else
                    break;
            }

            for (i = s; i < tris->size; ++i)
            {
                triangle_t *t = arraylist_get(tris, i);

                if (point_in_triangle((point_t) {x, y}, *t))
                {
                    break;
                }
            }

            data_t *d = arraylist_get(data, i);

            d->r += image.data[x + y * image.w].r;
            d->g += image.data[x + y * image.w].g;
            d->b += image.data[x + y * image.w].b;
            d->cnt += 1;
        }
    }

    arraylist_p pixels = arraylist_make(NULL);

    for (i = 0 ; i < data->size; ++i)
    {
        data_t *d = arraylist_get(data, i);
        pixel_t *p = malloc(sizeof(pixel_t));

        if (d->cnt == 0) d->cnt = 1;

        p->r = d->r / d->cnt;
        p->g = d->g / d->cnt;
        p->b = d->b / d->cnt;

        arraylist_append(pixels, p);

        free(d);
    }

    arraylist_destroy(data);

    return pixels;
}

void set_colors(arraylist_p colors, arraylist_p tris, ppm_t image)
{
    int x, y, i, s = 0;
    for (x = 0; x < image.w; ++x) 
    {
        for (y = 0; y < image.h; ++y)
        {
            while (s < tris->size)
            {
                point_t *t = *((triangle_t*) arraylist_get(tris, s));

                int t_right = max(t[0].x, max(t[1].x, t[2].x));

                if (t_right < x)
                    ++s;
                else
                    break;
            }

            for (i = s; i < tris->size; ++i)
            {
                triangle_t *t = arraylist_get(tris, i);

                if (point_in_triangle((point_t) {x, y}, *t))
                {
                    break;
                }
            }

            pixel_t *p = arraylist_get(colors, i);

            image.data[x + y * image.w].r = p->r;
            image.data[x + y * image.w].g = p->g;
            image.data[x + y * image.w].b = p->b;
        }
    }
}