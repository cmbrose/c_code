/*
 *	FILE: bitvector.c
 *	AUTHOR: Caleb Brose
 *
 *	USE: Defines useful functions for loading, maniulating, 
 *       and writing images.
 */

#include <stdio.h>
#include <stdlib.h>
#include "image_utils.h"

// From https://stackoverflow.com/questions/687261/converting-rgb-to-grayscale-intensity
#define R_GRAYSCALE_WEIGHT 0.2989
#define G_GRAYSCALE_WEIGHT 0.5870
#define B_GRAYSCALE_WEIGHT 0.1140

#define BLACKANDWHITE_THRESHOLD (160)

#define PIXEL_TO_GRAYSCALE(P) ((unsigned char) (R_GRAYSCALE_WEIGHT * (P).r + G_GRAYSCALE_WEIGHT * (P).g + B_GRAYSCALE_WEIGHT * (P).b))

//----------------------------------------------------
image_p image_make(int w, int h)
{
    image_p img = malloc(sizeof(pixel_t));
    image_init(img, w, h);
    return img;
}

//----------------------------------------------------
void image_init(image_p img, int w, int h)
{
    img->w = w;
    img->h = h;

    img->pixels = malloc(sizeof(pixel_t*) * h);

    int i;
    for (i = 0; i < h; ++i)
    {
        img->pixels[i] = malloc(sizeof(pixel_t) * w);
    }
}

//----------------------------------------------------
void image_destroy(image_p img)
{
    int i;
    for (i = 0; i < img->h; ++i)
    {
        free(img->pixels[i]);
    }

    free(img->pixels);
    free(img);
}

//----------------------------------------------------
void set_pixel(image_p img, pixel_t value, int x, int y)
{
    img->pixels[y][x] = value;
}

//----------------------------------------------------
pixel_t get_pixel(image_p img, int x, int y)
{
    return img->pixels[y][x];
}

//----------------------------------------------------
void convert_to_grayscale(image_p img)
{
    int i;
    for (i = 0; i < img->h; ++i)
    {
        int j;
        for (j = 0; j < img->w; ++j)
        {
            pixel_t p = get_pixel(img, j, i);
            
            p.r = p.g = p.b = PIXEL_TO_GRAYSCALE(p);

            set_pixel(img, p, j, i);
        }
    }
}

//----------------------------------------------------
void convert_to_black_and_white(image_p img)
{
    int i;
    for (i = 0; i < img->h; ++i)
    {
        int j;
        for (j = 0; j < img->w; ++j)
        {
            pixel_t p = get_pixel(img, j, i);
            
            p.r = p.g = p.b = PIXEL_TO_GRAYSCALE(p) < BLACKANDWHITE_THRESHOLD ? 0 : 255;

            set_pixel(img, p, j, i);
        }
    }
}

//----------------------------------------------------
void crop_image(image_p img, int x, int y, int w, int h)
{    
    int i;
    for (i = 0; i < h; ++i)
    {
        int j;
        for (j = 0; j < w; ++j)
        {
            pixel_t p = get_pixel(img, x + j, y + i);            
            set_pixel(img, p, j, i);
        }

        img->pixels[i] = realloc(img->pixels[i], sizeof(pixel_t) * w);
    }

    img->pixels = realloc(img->pixels, sizeof(pixel_t*) * h);

    img->w = w;
    img->h = h;
}

//----------------------------------------------------
image_p load_from_ppm(char *filename)
{
    FILE *file = fopen(filename, "rb");

    int w, h;
    fscanf(file, "P6 %d %d 255", &w, &h);
    
    image_p img = image_make(w, h);

    int i;
    for (i = 0; i < h; ++i)
    {
        int j;
        for (j = 0; j < w; ++j)
        {
            pixel_t p;
            fread(&p, sizeof(unsigned char), sizeof(pixel_t), file);
            set_pixel(img, p, j, i);
        }
    }
    
    fclose(file);

    return img;
}

//----------------------------------------------------
void output_to_ppm(image_p img, char *filename)
{
    FILE *file = fopen(filename, "wb");

    fprintf(file, "P6 %d %d 255\n", img->w, img->h);
        
    int i;
    for (i = 0; i < img->h; ++i)
    {
        int j;
        for (j = 0; j < img->w; ++j)
        {
            pixel_t p = get_pixel(img, j, i);
            fwrite(&p, sizeof(unsigned char), sizeof(pixel_t), file);
        }
    }
    
    fclose(file);
}