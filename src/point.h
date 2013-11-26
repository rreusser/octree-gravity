#ifndef __POINT_H__
#define __POINT_H__

typedef struct {
    real x,y,z,m;
} point;

point* copy_point( point* src );

point add(point a, real x, real y, real z);

void write_point( FILE* f, const char* s, point* p );

#endif /*__POINT_H__*/
