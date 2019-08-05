#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "arraylist.h"
#include "image_utils.h"

#define PI 3.14159265

#define PEG_CNT 200
#define THREAD_CNT 10000
#define THREAD_DARKNESS 50

#define signof(X) ((X) > 0) - ((X) < 0)
#define BOUND(X, m, M) { if ((X) < (m)) { (X) = (m); } else if ((X) > (M)) { (X) = (M); } }

typedef struct
{
    int x;
    int y;
} peg_t;

int crop_circle(image_p img);
arraylist_p make_pegs(int cnt, int radius);
image_p make_thread_image(image_p target, int radius);
int coordinate_in_circle(int x, int y, int radius);
int select_next_peg_greedy(int current_peg, image_p target, image_p current_img, arraylist_p pegs);
void walk_line(void* data, int x1, int y1, int x2, int y2, void(*callback)(void*, int, int));

int main(int argc, char *argv[])
{
    image_p target = load_from_ppm(argv[1]);

    int radius = crop_circle(target);

    convert_to_grayscale(target);

    image_p result = make_thread_image(target, radius);

    output_to_ppm(result, argv[2]);

    image_destroy(target);
    image_destroy(result);

    return 0;
}

int coordinate_in_circle(int x, int y, int radius)
{
    int dist_x = radius - x;
    int dist_y = radius - y;

    return dist_x * dist_x + dist_y * dist_y <= radius * radius;
}

int crop_circle(image_p img)
{
    int mid_x = img->w / 2;
    int mid_y = img->h / 2;
    int r = mid_x < mid_y ? mid_x : mid_y;

    int shift_x = (mid_x - r) / 2;
    int shift_y = (mid_y - r) / 2;

    crop_image(img, shift_x, shift_y, r * 2, r * 2);

    pixel_t black = {0, 0, 0};

    int i;
    for (i = 0; i < img->w; ++i)
    {
        int j;
        for (j = 0; j < img->h; ++j)
        {
            if (!coordinate_in_circle(i, j, r))
            {
                set_pixel(img, black, i, j);
            }
        }
    }

    return r;
}

arraylist_p make_pegs(int cnt, int radius)
{
    arraylist_p pegs = arraylist_make(NULL, sizeof(peg_t));

    int i;
    for (i = 0; i < cnt; ++i)
    {
        int x = (int) (cos(i * (2 * PI) / cnt) * radius) + radius - 1;
        int y = (int) (sin(i * (2 * PI) / cnt) * radius) + radius - 1;

        BOUND(x, 0, radius * 2 - 1);
        BOUND(y, 0, radius * 2 - 1);

        peg_t peg = { x, y };

        arraylist_append(pegs, &peg);
    }

    return pegs;
}

void overlap_threads(void* img, int x, int y)
{
    pixel_t p = get_pixel((image_p)img, x, y);

    p.r = p.r < THREAD_DARKNESS ? 0 : p.r - THREAD_DARKNESS;
    p.g = p.g < THREAD_DARKNESS ? 0 : p.g - THREAD_DARKNESS;
    p.b = p.b < THREAD_DARKNESS ? 0 : p.b - THREAD_DARKNESS;

    set_pixel((image_p)img, p, x, y);
}

image_p make_thread_image(image_p target, int radius)
{
    arraylist_p pegs = make_pegs(PEG_CNT, radius);

    image_p res = image_make(target->w, target->h);

    pixel_t black = {0, 0, 0};
    pixel_t white = {255, 255, 255};

    int i;
    for (i = 0; i < target->w; ++i)
    {
        int j;
        for (j = 0; j < target->h; ++j)
        {
            if (!coordinate_in_circle(i, j, radius))
            {
                set_pixel(res, black, i, j);
            }
            else
            {
                set_pixel(res, white, i, j);
            }
        }
    }

    int current_peg = 0;

    for (i = 0; i < THREAD_CNT; ++i)
    {
        int next_peg = select_next_peg_greedy(current_peg, target, res, pegs);
        
        peg_t p1, p2;
        arraylist_get(pegs, current_peg, &p1);
        arraylist_get(pegs, next_peg, &p2);
        
        walk_line(res, p1.x, p1.y, p2.x, p2.y, overlap_threads);
        
        current_peg = next_peg;
    }

    return res;
}

typedef struct 
{
    image_p target;
    image_p current_img;
    int *score;  
} greedy_peg_selection_packet_t;

void greedy_scoring_callback(void* data, int x, int y)
{
    greedy_peg_selection_packet_t *packet = (greedy_peg_selection_packet_t*)data;

    pixel_t target_p = get_pixel(packet->target, x, y);
    pixel_t current_p = get_pixel(packet->current_img, x, y);

    *(packet->score) += abs(target_p.r - (current_p.r - THREAD_DARKNESS));
}

int select_next_peg_greedy(int current_peg_idx, image_p target, image_p current_img, arraylist_p pegs)
{
    static int diff = 0;

    int i, j;

    if (diff == 0)
    {
        for (i = 0; i < target->w; ++i)
        {
            for (j = 0; j < target->h; ++j)
            {
                diff += abs(get_pixel(target, i, j).r - get_pixel(current_img, i, j).r);
            }
        }
    }

    peg_t current_peg, test_peg;
    arraylist_get(pegs, current_peg_idx, &current_peg);

    int best_peg, best_score = INT_MAX;

    for (i = 0; i < arraylist_size(pegs); ++i)
    {
        if (i == current_peg_idx)
        {
            continue;
        }
        
        arraylist_get(pegs, i, &test_peg);
        
        int score = 0;
        greedy_peg_selection_packet_t packet = { target, current_img, &score };
        walk_line(&packet, current_peg.x, current_peg.y, test_peg.x, test_peg.y, greedy_scoring_callback);
        
        int test_diff = diff - score;

        if (test_diff < best_score)
        {
            best_score = test_diff;
            best_peg = i;
        }
    }

    return best_peg;
}

void walk_line(void* data, int x1, int y1, int x2, int y2, void(*callback)(void*, int, int))
{
    int del_x, del_y;
    int f_x, f_y;
    int a, b, c, i;

    del_x = abs(x2 - x1);
    f_x = signof(x2 - x1);

    del_y = abs(y2 - y1);
    f_y = signof(y2 - y1);

    if (del_x > del_y)
    {
        a = del_y + del_y;
        c = a - del_x;
        b = c - del_x;
        for (i = 0; i <= del_x; i++) 
        {
            callback(data, x1, y1);

            x1 += f_x;

            if (c < 0) 
            {
                c += a;
            }
            else
            {
                c += b;
                y1 += f_y;
            }
        }
    } 
    else
    {
        a = del_x + del_x;
        c = a - del_y;
        b = c - del_y;
        for (i = 0; i <= del_y; i++) 
        {
            callback(data, x1, y1);

            y1 += f_y;

            if (c < 0) 
            {
                c += a;
            } 
            else 
            {
                c += b;
                x1 += f_x;
            }
        }
    }
}