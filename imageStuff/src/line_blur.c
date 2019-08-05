#include <stdlib.h>
#include <time.h>
#include "ppm.h"
#include "options.h"

void line_blur(ppm_t src, ppm_t res, method_options_t opts)
{
	line_blur_options_t lb_opts = opts.line_blur;
	
	srand(time(NULL));

	int min_width = lb_opts.min_width;
	int min_height = lb_opts.min_height;
	int max_width = lb_opts.max_width;
	int max_height = lb_opts.max_height;
	
	int x, y;

	char **is_set;

	is_set = malloc(sizeof(char*) * src.h);
	for (y = 0; y < src.h; ++y)
	{
		is_set[y] = malloc(sizeof(char) * src.w);
		for (x = 0; x < src.w; ++x)
		{
			is_set[y][x] = 0;
		}
	}

	int i, j, dx, dy;
	pixel_t color;

	for (y = 0; y < src.h; ++y)
	{
		for (x = 0; x < src.w; ++x)
		{
			if (is_set[y][x])
			{
				continue;
			}

			color = ppm_get_pixel(src, x, y);

			dx = rand() % (max_width - min_width + 1) + min_width;
			dy = rand() % (max_height - min_height + 1) + min_height;

			for (j = y; j < y + dy && j < res.h; ++j)
			{
				for (i = x; i < x + dx && i < res.w; ++i)
				{
					ppm_set_pixel(res, i, j, color);
					is_set[j][i] = 1;
				}
			}
		}
		free(is_set[y]);
	}
	free(is_set);
}