#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RATIO		2

#define WIDTH 		(1960 / RATIO)
#define HEIGHT 		(1024 / RATIO)
#define MAX_COLOR	255

#define HEADER		"P6 %d %d %d\n"
#define P_PIXEL		"%c%c%c"

#define MIN_BAR_H	((int) (HEIGHT * 0.005))
#define MAX_BAR_H	((int) (HEIGHT * 0.025))

#define MIN_BAR_W	((int) (WIDTH * 0))
#define MAX_BAR_W	((int) (WIDTH * 1)) // Let it go a bit off the edge

#define BOUND(X, m, M)	((X < m ? m : (X > M ? M : X)))

void rand_color(unsigned char p[3]);
void calc_bg_color(int y, unsigned char p[3]);
void copy_color(unsigned char d[3], unsigned char s[3]);
void try_modifiers(int cnt, char* args[]);
void do_help();

unsigned char gradient_list[2][3];

float variance[] = { 1.0, 1.0, 1.0 };
char var_set = 0;

int main(int argc, char* argv[])
{
	if ( argc < 3 )
	{
		if ( argc == 2 && 0 == strcmp(argv[1], "-h") ) 
			do_help();
		else
			printf("Need more info.\n");
		return;
	}
	
	srand(time(NULL));
	
	rand_color(gradient_list[0]);
	copy_color(gradient_list[1], gradient_list[0]);
	
	char* path = argv[1];
	int iters  = atoi(argv[2]);
	
	// Parse command line stuff
	try_modifiers(argc - 3, &(argv[3]));
	
	// Copy bg color onto image
	int x, y, i;
	unsigned char img[HEIGHT][WIDTH][3];
	
	for ( y = 0; y < HEIGHT; ++y )
	{
		unsigned char p[3];
		calc_bg_color(y, p);
		for ( x = 0; x < WIDTH; ++x )
		{
			copy_color(img[y][x], p);
		}
	}
	
	// Make bars
	while ( iters-- )
	{
		// Get bar shape and loc
		int w = rand() % (MAX_BAR_W - MIN_BAR_W) + MIN_BAR_W;
		int h = rand() % (MAX_BAR_H - MIN_BAR_H) + MIN_BAR_H;
		
		int x_dir = rand() % 2 ? 1 : -1;
		int x0 = x_dir == 1 ? 0 : WIDTH - 1;
		int y0 = rand() % (HEIGHT - h);
		
		unsigned char color[3], grad[3];
		calc_bg_color(y0, grad);
		
		// Do variance bounding
		for ( i = 0; i < 3; ++i )
		{
			int r = MAX_COLOR * variance[i];
			
			int min = BOUND(grad[i] - r, 0, MAX_COLOR);
			int max = BOUND(grad[i] + r, 0, MAX_COLOR);
			
			color[i] = rand() % (max - min + 1) + min;
		}
		
		// Set color to use
		for ( x = 0; x < w; ++x )
			for ( y = 0; y < h; ++y )
				copy_color(img[y + y0][x0 + x_dir * x], color);
	}
	
	// Print image
	FILE* file = fopen(path, "wb");
	fprintf(file, HEADER, WIDTH, HEIGHT, MAX_COLOR);
	
	for ( y = 0; y < HEIGHT; ++y )
	{
		for ( x = 0; x < WIDTH; ++x )
		{
			//fwrite(img[y][x], 1, 3, file);
			fprintf(file, P_PIXEL, img[y][x][0], img[y][x][1], img[y][x][2]);
		}
	}
	
	fclose(file);
}

void rand_color(unsigned char p[3])
{
	p[0] = rand() % MAX_COLOR;
	p[1] = rand() % MAX_COLOR;
	p[2] = rand() % MAX_COLOR;
}

void copy_color(unsigned char d[3], unsigned char s[3])
{
	d[0] = s[0];
	d[1] = s[1];
	d[2] = s[2];
}

void calc_bg_color(int y, unsigned char p[3])
{
	float weight_top  = 1.0 * (HEIGHT - y) / HEIGHT;
	float weight_bott = 1.0 * y / HEIGHT;
	int i;
	
	for ( i = 0; i < 3; ++i )
		p[i] = gradient_list[0][i] * weight_top + gradient_list[1][i] * weight_bott;
}

void do_help()
{
	printf( "Modifiers:\n\n" \
		"-h: 					Help. Where you are now.\n" \
		"-b R G B: 				Background. Sets background to given value.  Overwrites gradient.\n" \
		"-g R1 G1 B1 R2 G2 B2:	Gradient. Sets a gradient with C1 at the top and C2 at the bottom.\n" \
		"-gr:					Gradient Random.  Applies a random gradient to the image.\n" \
		"-v R G B:				Variance. Used to bound the used colors to a percentage of the background's value.\n" \
		"-va V					Variance All. Sets all variance values to the given value.\n" );
}

void try_modifiers(int cnt, char* args[])
{
	int i = 0, j;
	while ( i < cnt )
	{
		int rem = cnt - i;
		char* curr = args[i];
		
		// -h
		if ( 0 == strcmp("-h", curr) )
		{
			do_help();
			i += 1;
		}
		
		// -b
		else if ( 0 == strcmp("-b", curr) )
		{
			if ( rem < 4 ) 
				printf("WARNING: Not enough params for -b. Ignoring.\n");
			else
			{
				unsigned char color[3];
				for ( j = 0; j < 3; ++j )
				{
					unsigned char val = atoi(args[i+j+1]);
					color[j] = BOUND(val, 0, MAX_COLOR);
				}
				copy_color(gradient_list[0], color);
				copy_color(gradient_list[1], color);
			}
			i += 4;
		}
		
		// -g
		else if ( 0 == strcmp("-g", curr) )
		{
			if ( rem < 7 ) 
				printf("WARNING: Not enough params for -g. Ignoring.\n");
			else
			{
				int k;
				unsigned char color[3];
				for ( k = 0; k < 2; ++k )
				{
					for ( j = 0; j < 3; ++j )
					{
						int idx = i + j + 3 * k + 1; // Base offset + index + Iteration Offset + 1
						unsigned char val = atoi(args[idx]);
						color[j] = BOUND(val, 0, MAX_COLOR);
					}
					copy_color(gradient_list[k], color);
				}
			}
			i += 7;
		}
		
		// -gr
		else if ( 0 == strcmp("-gr", curr) )
		{
			unsigned char color[3];
			for ( j = 0; j < 2; ++j )
			{
				rand_color(color);
				copy_color(gradient_list[j], color);
			}
			i += 1;
		}
		
		// -v
		else if ( 0 == strcmp("-v", curr) )
		{
			if ( rem < 4 ) 
				printf("WARNING: Not enough params for -v. Ignoring.\n");
			else
			{
				var_set = 1;
				for ( j = 0; j < 3; ++j )
				{
					float val;
					sscanf(args[i+j+1], "%f", &val);
					variance[j] = BOUND(val, 0, 1.0);
				}
			}
			i += 4;
		}
		
		// -va
		else if ( 0 == strcmp("-va", curr) )
		{
			if ( rem < 2 ) 
				printf("WARNING: Not enough params for -va. Ignoring.\n");
			else
			{
				var_set = 1;
				float val;
				sscanf(args[i+1], "%f", &val);
				variance[0] = variance[1] = variance[2] = BOUND(val, 0, 1.0);
			}
			i += 2;
		}
	}
}

