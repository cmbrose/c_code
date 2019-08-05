#ifndef PROGRESS
#define PROGRESS

void setup_progress(char *s, float max);

void set_progress(float current);

void incr_progress();

void incr_progress_by(float incr);

void complete_progress();

#endif