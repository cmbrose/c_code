#ifndef DEBUG_H

#ifdef DEBUG
#define DEBUG_DEF 1
#else
#define DEBUG_DEF 0
#endif

#define DEBUG_INFO(format, ...) { if (DEBUG_DEF) { printf(format, ##__VA_ARGS__); } }

#endif // DEBUG_H