#ifndef GENERATORS_H
#define GENERATORS_H

#include "options.h"

void k_means(ppm_t src, ppm_t res, method_options_t opts);

void triangles(ppm_t src, ppm_t res, method_options_t opts);

void comic(ppm_t src, ppm_t res, method_options_t opts);

void line_blur(ppm_t src, ppm_t res, method_options_t opts);

#endif // GENERATORS_H
