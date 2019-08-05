#include <stdlib.h>
#include "ppm.h"
#include "options.h"

void comic(ppm_t src, ppm_t res, method_options_t opts)
{
	comic_options_t c_opts = opts.comic;
	
	int radius = c_opts.radius;
	int i, j;
	char is_even_row = 1;

	for (i = 0; i < ppm_pixel_cnt(src); ++i)
	{
		res.data[i].r = 255;
		res.data[i].g = 255;
		res.data[i].b = 255;
	}

	for (i = 0; i < src.h / radius / 2 - 1; ++i)
	{
		int y = (i + 1) * radius * 2;

		for (j = 0; j < src.w / radius / 2 - 1; ++j)
		{
			int x = (j + 1) * radius * 2 + (is_even_row ? 0 : radius);

			int r = 0, g = 0, b = 0, cnt = 0;

			int dx, dy;
			for (dx = -radius; dx <= radius; ++dx)
			{
				for (dy = -radius; dy <= radius; ++dy)
				{
					if (dx * dx + dy * dy > radius * radius)
					{
						continue;
					}

					pixel_t pix = ppm_get_pixel(src, x + dx, y + dy);

					r += pix.r;
					g += pix.g;
					b += pix.b;
					cnt += 1;
				}	
			}

			pixel_t avg = (pixel_t) {r / cnt, g / cnt, b / cnt};

			for (dx = -radius; dx <= radius; ++dx)
			{
				for (dy = -radius; dy <= radius; ++dy)
				{
					if (dx * dx + dy * dy > radius * radius)
					{
						continue;
					}

					ppm_set_pixel(res, x + dx, y + dy, avg);
				}	
			}
		}

		is_even_row = !is_even_row;
	}
}