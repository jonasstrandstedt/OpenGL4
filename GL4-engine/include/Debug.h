#ifndef DEBUG_H
#define DEBUG_H

#include <cstdio>

#ifndef NDEBUG
   #define LOG( x, ... ) printf( x, ##__VA_ARGS__)
   #define ERRLOG( x, ... ) fprintf(stderr, x, ##__VA_ARGS__)
#else
   #define LOG( x, ...)
   #define ERRLOG( x, ... )
#endif

#endif