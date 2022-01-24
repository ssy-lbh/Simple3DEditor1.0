#ifndef NULL
#define NULL __null
#endif

#ifdef _WIN64
typedef unsigned long long size_t;
#else
typedef unsigned int size_t;
#endif