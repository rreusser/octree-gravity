#ifndef __COMMON_H__
#define __COMMON_H__

typedef double real;


#define OCTREE_GRAVITY
//#define TWO_GALAXIES

#define OPENGL

#define SAVE_DATA



#define MIN(A,B) ((A)<(B)?(A):(B))
#define MAX(A,B) ((A)<(B)?(B):(A))
#define BOUND(A,B,C) MIN(MAX(A,B),C)
#define ABS(A) ((A)<0?(-(A)):(A))
#define SQR(A) ((A)*(A))
#define CUBE(A) ((A)*(A)*(A))
#define SIGN(A) ((A<0.0)?(-1):(1))

void ArrayToGnuplot( const char* fname, real* x, real* y, const int n );

void GetArrayMinMax( real* a, const int n, real* min, real* max);

void GetIntArrayMinMax( int* a, const int n, int* min, int* max);

void dbgp( const char* s, void* ptr );

void create_filename(char* dest, const char* prefix, int i, int digits, const char* suffix );

#endif /*__COMMON_H__*/
