#include <stdlib.h>
#include <time.h>
#include "ppm.h"
#include "options.h"

#define SQ_DIFF_PIX(X, Y) (((X).r - (Y).r) * ((X).r - (Y).r) + ((X).g - (Y).g) * ((X).g - (Y).g) + ((X).b - (Y).b) * ((X).b - (Y).b))
#define SQ_DIFF_COLOR(X, Y) (((X) - (Y)) * ((X) - (Y)))

void k_means(ppm_t src, ppm_t res, method_options_t opts)
{
	k_means_options_t km_opts = opts.k_means;
	
    srand(time(NULL));

    int *group_of;
    int k = km_opts.k;
	int pix_cnt = ppm_pixel_cnt(src);

    struct 
    {
        pixel_t mean;
        int num_pix;
        long long sum[3];
    } *group_data;

    int i, g;

    group_of = calloc(sizeof(int), pix_cnt);
    group_data = calloc(sizeof(*group_data), k);

    for (i = 0; i < k; ++i) 
	{
		int r = rand() % pix_cnt;

        group_data[i].mean = src.data[r];
	}

    int change = 0;

    do
    {
        change = 0;

        for (i = 0; i < pix_cnt; ++i)
        {       
            int min_group = 0;
            int min_dist = SQ_DIFF_PIX(group_data[0].mean, src.data[i]);

            for (g = 1; g < k; ++g) 
            {
                int dist = SQ_DIFF_PIX(group_data[g].mean, src.data[i]);

                if (dist < min_dist) 
                {
                    min_dist = dist;
                    min_group = g;
                }
            }

            if (min_group != group_of[i]) 
            {
                group_of[i] = min_group;
                change = 1;
            }

            group_data[min_group].num_pix += 1;
            group_data[min_group].sum[0] += src.data[i].r;
            group_data[min_group].sum[1] += src.data[i].g;
            group_data[min_group].sum[2] += src.data[i].b;
        }

        for (i = 0; i < k; ++i) 
        {
            if (group_data[i].num_pix > 0)
            {
                group_data[i].mean.r = group_data[i].sum[0] / group_data[i].num_pix;
                group_data[i].mean.g = group_data[i].sum[1] / group_data[i].num_pix;
                group_data[i].mean.b = group_data[i].sum[2] / group_data[i].num_pix;
            }
            else 
            {
                group_data[i].mean = (pixel_t) {0,0,0};
            }

            group_data[i].num_pix = 0;
            group_data[i].sum[0] = group_data[i].sum[1] = group_data[i].sum[2] = 0;
        }
    }
    while (change);

    for (i = 0; i < pix_cnt; ++i)
    {
    	res.data[i] = group_data[group_of[i]].mean;
    }
}