#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ppm.h"

ppm_t* ppm_make(int width, int height)
{
    ppm_t *ppm = malloc(sizeof(ppm_t));
    ppm->w = width;
    ppm->h = height;
    ppm->data = malloc(3 * width * height);

    return ppm;
}

ppm_t* ppm_make_from_file(char *filename)
{
    FILE *file = fopen(filename, "rb");
    ppm_t *ppm;
    char buf[64];
    int w, h;

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

    sscanf(buf, "%d %d", &w, &h);

    fgets(buf, 64, file);

    ppm = ppm_make(w, h);

    fread(ppm->data, 1, 3 * ppm->w * ppm->h, file);

    fclose(file);

    return ppm;
}

pixel_t ppm_get_pixel(ppm_t image, int x, int y)
{
    return image.data[x + y * image.w];
}

void ppm_set_pixel(ppm_t image, int x, int y, pixel_t pix)
{
    image.data[x + y * image.w].r = pix.r;
    image.data[x + y * image.w].g = pix.g;
    image.data[x + y * image.w].b = pix.b;
}

int ppm_pixel_cnt(ppm_t image)
{
    return image.w * image.h;
}

void ppm_write_to_file(ppm_t image, char *filename)
{
    FILE *file = fopen(filename, "wb");

    fprintf(file, "P6\n%d %d\n255\n", image.w, image.h);

    fwrite(image.data, 1, 3 * image.w * image.h, file);
    
    fclose(file);
}

void ppm_free(ppm_t *ppm)
{
    if (ppm == NULL)
    {
        return;
    }

    if (ppm->data != NULL)
    {
        free(ppm->data);
    }

    free(ppm);
}