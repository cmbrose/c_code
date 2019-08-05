#ifndef OPTIONS_H
#define OPTIONS_H

#include "ppm.h"

typedef struct 
{
	int min_width;
	int min_height;
	int max_width;
	int max_height;
} line_blur_options_t;

typedef struct 
{
	int k;
} k_means_options_t;

typedef struct 
{
	int count;
} triangles_options_t;

typedef struct 
{
	int radius;
} comic_options_t;

typedef union
{
	line_blur_options_t line_blur;
	k_means_options_t k_means;
	triangles_options_t triangles;
	comic_options_t comic;
} method_options_t;

typedef struct
{
	method_options_t method_options;
	
	char *in_file;
	char *out_file;
	
	void (*method)(ppm_t, ppm_t, method_options_t);
	
} options_t;

int parse_options(int cnt, char *args[], options_t *opts);

#endif // OPTIONS_H