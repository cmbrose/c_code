#include "ppm.h"
#include "generators.h"
#include "options.h"

int main(int argc, char *argv[])
{
	ppm_t *img, *result;
	options_t opts;
	
	if (parse_options(argc - 1, argv + 1, &opts) != 0)
	{
		return -1;
	}
	
	img = ppm_make_from_file(opts.in_file);
	result = ppm_make(img->w, img->h);

	opts.method(*img, *result, opts.method_options);
	
	ppm_write_to_file(*result, opts.out_file);

	ppm_free(img);
	ppm_free(result);

    return 0;
}