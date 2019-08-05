#ifndef PROGRESS
#define PROGRESS

#include <string>

void setup_progress(std::string s, float max);

void set_progress(float current);

void incr_progress();

void incr_progress_by(float incr);

void complete_progress();

#endif