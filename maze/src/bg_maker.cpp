#include <fstream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <vector>
#include "prim_maze.h"
#include "progress.h"
#include "shapes.h"
#include "debug.h"

#define DHEIGHT              1080
#define DWIDTH               1920
#define DVAR                 1000
#define DLOOPS               3
#define DREVERSE             true
#define DOUT_IMAGE           true
#define DWRAP_MAZE           false
#define DSTART_COLOR         0x2dcc70
#define DEND_COLOR           0x003366
#define DCOLORS_FROM_CORNERS false
#define DSHAPE               "RECT"

struct
{
    int  height;
    int  width;
    int  variance;
    int  num_loops;
    bool reverse;
    bool output_image;
    bool wrap_maze;
    std::vector<int> colors;
    bool colors_from_corners;
    const char *shape_mode;
} opts;

struct
{
    char* backup_file;
    char* input_file;
    const char *output_file;
} files;

void output_to_backup(unsigned short** grid, std::string filename, int w, int h);
unsigned short** load_backup(std::string filename, int *w, int *h, unsigned short &max_dist);
void output_to_ppm(unsigned short** grid, std::string filename, int w, int h, unsigned short maxVal, shape s);
void int_to_color(int val, int maxVal, char* bytes, int start_color, int end_color);
void parse_args(int argc, char* argv[]);
void make_selected_shape(shape &s);
int parse_color(char *rgb);
void select_start_and_end_colors(int &start, int &end, int x, int y, shape s);

int main(int argc, char* argv[])
{
    static const int def_colors[] = { DSTART_COLOR, DEND_COLOR };
    std::vector<int> def_color_vec (def_colors, def_colors + sizeof(def_colors) / sizeof(def_colors[0]));
    opts  = { DHEIGHT, DWIDTH, DVAR, DLOOPS, DREVERSE, DOUT_IMAGE, DWRAP_MAZE, def_color_vec, DCOLORS_FROM_CORNERS, DSHAPE };
    files = { NULL,  NULL, "image.ppm" };

    parse_args(argc, argv);
    
    shape s;
    make_selected_shape(s);

    unsigned short maxDist = -1;
    unsigned short** grid = NULL;
    
    if (files.input_file != NULL)
    {
        grid = load_backup(files.input_file, &opts.width, &opts.height, maxDist);
    }
    else
    {
        grid = make_maze_in_shape(opts.height, opts.width, opts.variance, maxDist, opts.wrap_maze, s);
    }
    
    if (files.backup_file != NULL)
    {
        output_to_backup(grid, files.backup_file, opts.width, opts.height);
    }
        
    if (opts.output_image)
    {
        output_to_ppm(grid, std::string(files.output_file), opts.width, opts.height, maxDist, s);
    }
    
	return 0;
}

void make_selected_shape(shape &s)
{
    DEBUG_INFO("Setting shape to mode %s", opts.shape_mode);

    if (strcmp("TRI", opts.shape_mode) == 0)
    {
        make_equilateral_triangle(opts.width, opts.height, s);
    }
    else if (strcmp("RECT", opts.shape_mode) == 0)
    {
        make_rectangle(opts.width, opts.height, s);
    }
    else if (strcmp("PENT", opts.shape_mode) == 0)
    {
        make_pentagon(opts.width, opts.height, s);
    }
}

void int_to_color(unsigned short val, unsigned short maxVal, char* bytes, int start_color, int end_color)
{
	// Pixel not set, outside of the shape
	if (val == USHRT_MAX)
	{
		bytes[0] = bytes[1] = bytes[2] = 0;
		return;
	}
	
    double ratio = 1.0 * val / maxVal * opts.num_loops;

    int loopNumber = (int) ratio;
    double coef = ratio - loopNumber;

    int idx = 2;

    bool doGradRev = opts.reverse && (loopNumber & 1);

    int start = doGradRev ? start_color : end_color;
    int end = doGradRev ? end_color : start_color;

    while ( idx > -1 )
    {
        char byte = (start & 0xFF) * (1 - coef) + (end & 0xFF) * coef;

        bytes[idx--] = byte;
        start >>= 8;
        end >>= 8;
    }
}

void select_start_and_end_colors(int &start, int &end, int x, int y, shape s)
{
    if (!opts.colors_from_corners)
    {
        start = opts.colors[0];
        end = opts.colors[1];
        return;
    }

    int nearest_idx = 0;
    int second_nearest_idx = 1;

    point p = {x, y};

    int nearest_dist_sq = P_DIST_SQ(p, s.segments[nearest_idx].start);
    int second_nearest_dist_sq = P_DIST_SQ(p, s.segments[second_nearest_idx].start);

    if (nearest_dist_sq > second_nearest_dist_sq)
    {
        int tmp = nearest_dist_sq;
        nearest_dist_sq = second_nearest_dist_sq;
        second_nearest_dist_sq = tmp;

        nearest_idx = 1;
        second_nearest_idx = 0;
    }

    for (unsigned int i = 2; i < s.segments.size(); ++i)
    {
        point test_point = s.segments[i].start;
        int test_dist_sq = P_DIST_SQ(p, test_point);

        if (test_dist_sq < nearest_dist_sq)
        {
            second_nearest_idx = nearest_idx;
            second_nearest_dist_sq = nearest_dist_sq;

            nearest_idx = i;
            nearest_dist_sq = test_dist_sq;
        }
        else if (test_dist_sq < second_nearest_dist_sq)
        {
            second_nearest_idx = i;
            second_nearest_dist_sq = test_dist_sq;
        }
    }

    unsigned  min_idx = nearest_idx < second_nearest_idx ? nearest_idx : second_nearest_idx;
    unsigned int max_idx = nearest_idx > second_nearest_idx ? nearest_idx : second_nearest_idx;

    unsigned int idx1 = (min_idx & 1) == 0 && max_idx != s.segments.size() ? min_idx : max_idx;
    unsigned int idx2 = idx1 == min_idx ? max_idx : min_idx;

    start = opts.colors[idx1 % opts.colors.size()];
    end = opts.colors[idx2 % opts.colors.size()];
}

void output_to_ppm(unsigned short** grid, std::string filename, int w, int h, unsigned short maxVal, shape s)
{
    std::ofstream file;
    file.open(filename.c_str(), std::ios::binary);

    char buf[256];
    sprintf(buf, "P6 %d %d 255\n", w, h);

    file.write(buf, strlen(buf));

    for ( int i = 0; i < 4; ++i )
        buf[i] = 0;

    setup_progress("Writing image", w * h);
        
    int start_color, end_color;

    for ( int i = 0; i < h; ++i )
    {
        for ( int j = 0; j < w; ++j )
        {
            select_start_and_end_colors(start_color, end_color, j, i, s);

            int_to_color(grid[i][j], maxVal, buf, start_color, end_color);
            file.write(buf, 3);
            
            incr_progress();
        }
    }
    
    complete_progress();
    file.close();
}

void output_to_backup(unsigned short** grid, std::string filename, int w, int h)
{
    std::ofstream file;
    file.open(filename.c_str(), std::ios::binary);
    
    char buf[256];
    sprintf(buf, "%d %d\n", w, h);
    file.write(buf, strlen(buf));

    setup_progress("Writing backup", w * h);
        
    for ( int i = 0; i < h; ++i )
    {
        for ( int j = 0; j < w; ++j )
        {
            char buf[2];           
            *((short*) buf) = grid[i][j];
        
            file.write(buf, 2);
            incr_progress();
        }
    }
    
    complete_progress();
    file.close();
}

unsigned short** load_backup(std::string filename, int *w, int *h, unsigned short &max_dist)
{
    max_dist = 0;
    std::ifstream file;
    file.open(filename.c_str(), std::ios::binary | std::ios::in);
    
    std::string buf;
    std::getline(file, buf);
    sscanf(buf.c_str(), "%d %d\n", w, h);

    setup_progress("Reading backup", *w * *h);
    
    unsigned short** grid = (unsigned short**) malloc(*h * sizeof(unsigned short*));
        
    for ( int i = 0; i < *h; ++i )
    {
        grid[i] = (unsigned short*) malloc(*w * sizeof(unsigned short));
        for ( int j = 0; j < *w; ++j )
        {
            char buf[2];
            file.read(buf, 2);
            
            grid[i][j] = *((short*) buf);
            
            if (max_dist < grid[i][j])
                max_dist = grid[i][j];
            
            incr_progress();
        }
    }
    
    complete_progress();
    file.close();
    
    return grid;
}

void parse_args(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-h") == 0)
        {
            opts.height = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-w") == 0)
        {
            opts.width = atoi(argv[++i]);
        }        
        else if (strcmp(argv[i], "-l") == 0)
        {
            opts.num_loops = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-v") == 0)
        {
            opts.variance = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-c") == 0)
        {
            int cnt = atoi(argv[++i]);

            opts.colors.clear();

            for (int j = 0; j < cnt; ++j)
            {
                 opts.colors.push_back(parse_color(argv[++i]));
            }
        }
        else if (strcmp(argv[i], "-C") == 0)
        {
            opts.colors_from_corners = true;
        }
        else if (strcmp(argv[i], "-r") == 0)
        {
            opts.reverse = false;
        }
        else if (strcmp(argv[i], "-n") == 0)
        {
            opts.output_image = false;
        }
        else if (strcmp(argv[i], "-W") == 0)
        {
            opts.wrap_maze = true;
        }
        else if (strcmp(argv[i], "-S") == 0)
        {
            opts.shape_mode = argv[++i];
        }
        else if (strcmp(argv[i], "-b") == 0)
        {
            files.backup_file = argv[++i];
        }
        else if (strcmp(argv[i], "-i") == 0)
        {
            files.input_file = argv[++i];
        }
        else if (strcmp(argv[i], "-o") == 0)
        {
            files.output_file = argv[++i];
        }
    }
}

int parse_color(char *rgb)
{
    int color = 0;
    int idx_of_start = 0;

    for (int i = 0; rgb[i]; ++i)
    {
        if (rgb[i] == ',')
        {
            rgb[i] = 0;
            color = (color << 8) | (atoi(&(rgb[idx_of_start])) & 0xFF);
            rgb[i] = ',';
            idx_of_start = i + 1;
        }
    }

    // Last color in list
    color = (color << 8) | (atoi(&(rgb[idx_of_start])) & 0xFF);

    return color;
}
