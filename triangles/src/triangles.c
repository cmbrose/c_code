#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "triangulation.h"
#include "progress.h"

#define max(X, Y) ((X) > (Y) ? (X) : (Y))

#define DCOUNT 10

typedef struct
{
    int count;
    char* input_file;
    char* output_file;
} opts_t;

typedef struct 
{
    unsigned char r, g, b;
} pixel_t;

typedef struct 
{
    int w, h;
    pixel_t *pixels;
} ppm_t;

void read_file(ppm_t *image, char *filename);
void output_to_ppm(ppm_t image, char *filename);
void parse_args(int argc, char* argv[], opts_t *opts);
arraylist_p get_tris(int cnt, ppm_t image);
arraylist_p get_colors(arraylist_p tris, ppm_t image);
void set_colors(arraylist_p colors, arraylist_p tris, ppm_t image);

int main(int argc, char* argv[])
{
    opts_t opts = (opts_t) { DCOUNT, argv[1], "image.ppm" };

    parse_args(argc - 1, argv + 1, &opts);
    
    ppm_t image;
    read_file(&image, opts.input_file);

    arraylist_p tris = get_tris(opts.count, image);

    arraylist_p colors = get_colors(tris, image);

    set_colors(colors, tris, image);
        
    output_to_ppm(image, opts.output_file);
    
    return 0;
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

    setup_progress("Calculating colors", image.w * image.h);

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

            d->r += image.pixels[x + y * image.w].r;
            d->g += image.pixels[x + y * image.w].g;
            d->b += image.pixels[x + y * image.w].b;
            d->cnt += 1;

            incr_progress();
        }
    }

    complete_progress();

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
    setup_progress("Setting colors", image.w * image.h);

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

            image.pixels[x + y * image.w].r = p->r;
            image.pixels[x + y * image.w].g = p->g;
            image.pixels[x + y * image.w].b = p->b;

            incr_progress();
        }
    }

    complete_progress();
}

void read_file(ppm_t *image, char *filename)
{
    FILE *file = fopen(filename, "rb");
    char buf[64];

    fgets(buf, 64, file);
    if (buf[0] != 'P' || buf[1] != '6')
    {
        exit(0);
    }

    do
    {
        fgets(buf, 64, file);
    }
    while (buf[0] == '#');

    sscanf(buf, "%d %d", &image->w, &image->h);

    fgets(buf, 64, file);

    image->pixels = malloc(3 * image->w * image->h);

    fread(image->pixels, 1, 3 * image->w * image->h, file);

    fclose(file);
}

void output_to_ppm(ppm_t image, char *filename)
{
    FILE *file = fopen(filename, "wb");

    fprintf(file, "P6\n%d %d\n255\n", image.w, image.h);

    fwrite(image.pixels, 1, 3 * image.w * image.h, file);
    
    fclose(file);
}

void parse_args(int argc, char* argv[], opts_t *opts)
{
    int i;
    for (i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-n") == 0)
        {
            opts->count = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-o") == 0)
        {
            opts->output_file = argv[++i];
        }
    }
}
