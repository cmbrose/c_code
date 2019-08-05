#ifndef PPM_H
#define PPM_H

typedef struct 
{
    unsigned char r, g, b;
} pixel_t;

typedef struct 
{
    int w, h;
    pixel_t *data;
} ppm_t;

ppm_t* ppm_make_from_file(char *filename);

ppm_t* ppm_make(int width, int height);

pixel_t ppm_get_pixel(ppm_t image, int x, int y);

void ppm_set_pixel(ppm_t image, int x, int y, pixel_t pix);

int ppm_pixel_cnt(ppm_t image);

void ppm_write_to_file(ppm_t image, char *filename);

void ppm_free(ppm_t *ppm);

#endif // PPM_H