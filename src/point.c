#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "point.h"

point* copy_point( point* src ) {
    point* pt = malloc(sizeof(point));
    pt->x = src->x;
    pt->y = src->y;
    pt->z = src->z;
    pt->m = src->m;
    return pt;
}

point add(point a, real x, real y, real z) {
    point c; c.x = a.x+x; c.y = a.y+y; c.z = a.z+z; c.m = a.m;
    return c;
}

void write_point( FILE* f, const char* s, point* p ) {
    fprintf(f, "%s = (%f, %f, %f, %f)\n",s,p->x,p->y,p->z,p->m );
}
