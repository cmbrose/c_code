#include <string.h>
#include <stdlib.h>
#include "options.h"
#include "generators.h"

int parse_method_options(int cnt, char *args[], options_t *opts);

int parse_line_blur_options(int cnt, char *args[], line_blur_options_t *opts);
int parse_k_means_options(int cnt, char *args[], k_means_options_t *opts);
int parse_triangles_options(int cnt, char *args[], triangles_options_t *opts);
int parse_comic_options(int cnt, char *args[], comic_options_t *opts);

int parse_options(int cnt, char *args[], options_t *opts)
{
	if (cnt < 3)
	{
		return -1;
	}
	
	opts->in_file = args[0];
	opts->out_file = args[1];
	
	if (strstr("..", opts->in_file) != NULL || strstr("..", opts->out_file) != NULL)
		return -1;
	
	return parse_method_options(cnt - 2, args + 2, opts);
}

int parse_method_options(int cnt, char *args[], options_t *opts)
{
	char *method = args[0];
	
	if (strcmp("line_blur", method) == 0)
	{
		opts->method = &line_blur;
		return parse_line_blur_options(cnt - 1, args + 1, &(opts->method_options.line_blur));
	}
	else if (strcmp("k_means", method) == 0)
	{
		opts->method = &k_means;
		return parse_k_means_options(cnt - 1, args + 1, &(opts->method_options.k_means));
	}
	else if (strcmp("triangles", method) == 0)
	{
		opts->method = &triangles;
		return parse_triangles_options(cnt - 1, args + 1, &(opts->method_options.triangles));
	}
	else if (strcmp("comic", method) == 0)
	{
		opts->method = &comic;
		return parse_comic_options(cnt - 1, args + 1, &(opts->method_options.comic));
	}
	else 
	{
		return -1;
	}
}

int parse_line_blur_options(int cnt, char *args[], line_blur_options_t *opts)
{
	opts->min_width = 1;
	opts->min_height = 1;
	opts->max_width = 60;
	opts->max_height = 12;
	
	int i;
	for (i = 0; i < cnt - 1; i += 2)
	{
		if (strcmp("-w", args[i]) == 0)
		{
			opts->min_width = atoi(args[i + 1]);
		}
		else if (strcmp("-h", args[i]) == 0)
		{
			opts->min_height = atoi(args[i + 1]);
		}
		else if (strcmp("-W", args[i]) == 0)
		{
			opts->max_width = atoi(args[i + 1]);
		}
		else if (strcmp("-H", args[i]) == 0)
		{
			opts->max_height = atoi(args[i + 1]);
		}
		else 
		{
			return -1;
		}
	}
	
	if (opts->min_width < 0 || opts->max_width < opts->min_width || 
		opts->min_height < 0 || opts->max_height < opts->min_height)
	{
		return -1;
	}
	
	return 0;
}

int parse_k_means_options(int cnt, char *args[], k_means_options_t *opts)
{
	opts->k = 10;
	
	int i;
	for (i = 0; i < cnt - 1; i += 2)
	{
		if (strcmp("-k", args[i]) == 0)
		{
			opts->k = atoi(args[i + 1]);
		}
		else 
		{
			return -1;
		}
	}
	
	return 0;
}

int parse_triangles_options(int cnt, char *args[], triangles_options_t *opts)
{
	opts->count = 5000;
	
	int i;
	for (i = 0; i < cnt - 1; i += 2)
	{
		if (strcmp("-n", args[i]) == 0)
		{
			opts->count = atoi(args[i + 1]);
		}
		else 
		{
			return -1;
		}
	}
	
	return 0;
}


int parse_comic_options(int cnt, char *args[], comic_options_t *opts)
{
	opts->radius = 6;
	
	int i;
	for (i = 0; i < cnt - 1; i += 2)
	{
		if (strcmp("-r", args[i]) == 0)
		{
			opts->radius = atoi(args[i + 1]);
		}
		else 
		{
			return -1;
		}
	}
	
	return 0;
}
