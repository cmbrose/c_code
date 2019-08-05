#include "progress.h"
#include <stdio.h>

float final_val;
float current_val;

void setup_progress(char* s, float max)
{
    final_val = max;
    printf("\n%s\n", s);
    set_progress(0);
}

void set_progress(float current)
{
    current_val = current;
    
    static volatile int last = -1;
    float p = current_val / final_val;
    
    int perc = (int) p < 1 ? p * 100 : 100;
    
    fflush(stdout);
    
    if (perc == last) 
       return;
    last = perc;
    
    int i = 0;
    printf("[");
    
    for ( ; i < perc / 2; ++i ) 
        printf("=");
    
    if (perc % 2)
    {
        printf("-");
        ++i;
    }
    
    for ( ; i < 50; ++i ) 
        printf(" ");
        
    printf("] %3d%% (%.0f/%.0f)\r", perc, current_val, final_val); 
}

void complete_progress()
{
    set_progress(final_val);
    printf("\n");
}

void incr_progress()
{
    incr_progress_by(1);
}

void incr_progress_by(float incr)
{
    set_progress(current_val + incr);
}
