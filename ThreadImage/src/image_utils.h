#ifndef IMAGE_UTILS_H

// A single RGB pixel
typedef struct pixel
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} pixel_t;

// A 2-D grid of pixels which form the image
typedef struct image
{
    pixel_t** pixels;
	int w;
	int h;
} image_t, *image_p;

/*
 *	Mallocs a new image of the given dimensions.
 *		NOTE: This image is already initialized.
 *
 *	PARAMS:
 *		w - Width of the image.
 *      h - Height of the image.
 *
 *	RETURN:
 *		A new image of the given size.
 */
image_p image_make(int w, int h);

/*
 *	Initializes an image of the given size.
 *
 *	PARAMS:
 *		w - Width of the image.
 *      h - Height of the image.
 */
void image_init(image_p img, int w, int h);

/*
 *	Frees the memory of the given image.
 *
 *	PARAMS:
 *		img	- image to destroy.
 */
void image_destroy(image_p img);

/*
 *	Sets the color of the pixel at the given coordinate.
 *
 *	PARAMS:
 *		img   - image to modify.
 *		value - color to set the pixel to.
 *      x     - X-value of the specified coordinate.
 *      y     - Y-value of the specified coordinate.
 */
void set_pixel(image_p img, pixel_t value, int x, int y);

/*
 *	Gets the color of the pixel at the given coordinate.
 *
 *	PARAMS:
 *		img   - image to modify.
 *      x     - X-value of the specified coordinate.
 *      y     - Y-value of the specified coordinate.
 *
 *	RETURN:
 *		The color of the pixel at the given coordinate.
 */
pixel_t get_pixel(image_p img, int x, int y);

/*
 *	Converts the given image to grayscale colors.
 *      NOTE: The modification is in-place, the original
 *            image is lost.
 *
 *	PARAMS:
 *		img   - image to modify.
 */
void convert_to_grayscale(image_p img);

/*
 *	Converts the given image to black-and-white.
 *      NOTE: The modification is in-place, the original
 *            image is lost.
 *
 *	PARAMS:
 *		img   - image to modify.
 */
void convert_to_black_and_white(image_p img);

/*
 *	Crops the given image starting at the given point to 
 *  the given dimensions.
 *      NOTE: The modification is in-place, the original
 *            image is lost.
 *
 *	PARAMS:
 *		img - image to modify.
 *      x   - X-value of the top-left coordinate   
 *      y   - Y-value of the top-left coordinate  
 *      w   - width of the crop result  
 *      h   - height of the crop result
 */
void crop_image(image_p img, int x, int y, int w, int h);

/*
 *	Parses the given PPM file into an image.
 *
 *	PARAMS:
 *		filename - path to the PPM file.
 *
 *	RETURN:
 *		The image specified by the given file.
 */
image_p load_from_ppm(char *filename);

/*
 *	Writes the given image to a PPM file.
 *
 *	PARAMS:
 *      img      - the image to write.
 *		filename - path to the PPM file.
 */
void output_to_ppm(image_p img, char *filename);

#endif // IMAGE_UTILS_H