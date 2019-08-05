#ifndef DEBUG_H

#ifdef DEBUG
#define DEBUG_DEF 1
#else
#define DEBUG_DEF 0
#endif

#define DEBUG_INFO(format, args...) { if (DEBUG_DEF) { printf(format, args); } }

#endif // DEBUG_H