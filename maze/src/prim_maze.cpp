#include <stdlib.h>
#include <unordered_map>
#include <queue>
#include <vector>
#include <time.h>
#include <limits.h>
#include <stdio.h>
#include "prim_maze.h"
#include "progress.h"                    
                        
struct node
{
    short x, y;

    bool operator==(const node &other) const
    {
        return x == other.x && y == other.y;
    }
};

struct node_hash
{
    int operator()(const node& k) const
    {
        return k.x * 1000031 + k.y;
    }
};

struct node_comp
{
    bool operator()(const std::tuple<node, int, short>& l, const std::tuple<node, int, short>& r) const
    {
        return std::get<1>(l) < std::get<1>(r);
    }
};

typedef std::unordered_map<node, int, node_hash> adj_list;
typedef std::unordered_map<node, adj_list, node_hash> graph;

unsigned short** make_maze_from_graph
(
    short height, 
    short width, 
    int weight_variance, 
    unsigned short &max_dist,
    bool wrap_maze,
	graph g
);

void make_graph_from_shape
(
	short height,
	short width,
	int weight_variance, 
	graph &g,
	bool wrap,
	shape s
)
{
    setup_progress("Making nodes", height * width); 
    for ( short y = 0; y < height; ++y )
    {
        for ( short x = 0; x < width; ++x )
        {
			if (!point_in_shape((point) {x, y}, s))
				continue;
			
            g.insert(std::make_pair<node, adj_list>({ x, y }, adj_list()));
            incr_progress();
        }
    }
    complete_progress();
    
    setup_progress("Making edges", g.size());     
    graph::iterator iter;
    for ( iter = g.begin(); iter != g.end(); ++iter )
    {   
        node n = iter->first;
        
        short right_x, down_y;
        if (wrap)
        {
            right_x = (short) ((n.x + 1) % width);
            down_y  = (short) ((n.y + 1) % height);
        }
        else
        {
            right_x = n.x + 1;
            down_y  = n.y + 1;
        }
        
        graph::iterator r = g.find({ right_x, n.y });
        graph::iterator d = g.find({ n.x, down_y });

        int w_r = rand() % weight_variance;
        int w_d = rand() % weight_variance;

        if ( r != g.end() )
        {
            iter->second.insert(std::make_pair<>(r->first, w_r));
            r->second.insert(std::make_pair<>(n, w_r));
        }
        
        if ( d != g.end() )
        {
            iter->second.insert(std::make_pair<>(d->first, w_d));
            d->second.insert(std::make_pair<>(n, w_d));
        }
        
        incr_progress();
    }
    complete_progress();
}

unsigned short** make_maze
(
    short height, 
    short width, 
    int weight_variance, 
    unsigned short &max_dist,
    bool wrap_maze
) 
{   
    shape s;
	make_rectangle(width, height, s);
	
	return make_maze_in_shape(height, width, weight_variance, max_dist, wrap_maze, s);
}

unsigned short** make_maze_in_shape
(
    short height, 
    short width, 
    int weight_variance, 
    unsigned short &max_dist,
    bool wrap_maze,
	shape s
) 
{   
    graph g;
    make_graph_from_shape(height, width, weight_variance, g, wrap_maze, s);
	
	return make_maze_from_graph(height, width, weight_variance, max_dist, wrap_maze, g);
}

unsigned short** make_maze_from_graph
(
    short height, 
    short width, 
    int weight_variance, 
    unsigned short &max_dist,
    bool wrap_maze,
	graph g
) 
{
	max_dist = 0;
	srand(time(0));
		
    std::priority_queue<std::tuple<node, int, int>, std::vector<std::tuple<node, int, int>>, node_comp> queue;

    unsigned short** dists = (unsigned short**) malloc(height * sizeof(unsigned short*));
    for ( int y = 0; y < height; ++y )
    {
        dists[y] = (unsigned short*) malloc(width * sizeof(unsigned short));
        for ( int x = 0; x < width; ++x )
            dists[y][x] = USHRT_MAX;
    }

    setup_progress("Setting distances", g.size());
    unsigned int added_cnt = 0;
    
    short x, y;	
	graph::iterator n_in_g;
	do
	{
		x = rand() % width;
		y = rand() % height;
		
		n_in_g = g.find({x, y});
	}
	while (n_in_g == g.end());
	
    queue.push(std::make_tuple<node, int, short>({ x, y }, 0, 0));   
    
    while ( !queue.empty() && added_cnt < g.size() )
    {
        std::tuple<node, int, short> t = queue.top();
        queue.pop();

        node n = std::get<0>(t);

        if ( dists[n.y][n.x] != USHRT_MAX )
            continue;

        ++added_cnt;
            
        unsigned short dist = std::get<2>(t);
        int weighted_dist = std::get<1>(t);

        if ( dist > max_dist ) max_dist = dist;

        dists[n.y][n.x] = dist;

        adj_list adjs = g.find(n)->second;
        adj_list::iterator iter;
        for ( iter = adjs.begin(); iter != adjs.end(); ++iter )
        {
            node toadd = iter->first;
            if ( dists[toadd.y][toadd.x] != USHRT_MAX )
                continue;
                
            queue.push(std::make_tuple<>(iter->first, weighted_dist + iter->second, dist + 1));
        }
        
        incr_progress();
    }

    complete_progress(); 
    
    g.clear();
    
    return dists;
}


