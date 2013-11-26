#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include "camera.h"
#include "common.h"
#include "particles.h"
#include "point.h"
#include "octree_gravity.h"
#include "octree_draw.h"
#include "iterate.h"

extern particles* p;
extern octree* ot;
extern int frame;
extern int nextdisp;

void iterate() {
    int i;

    //fprintf(stderr,"Integration step: t=%f",p->t);

#ifdef SAVE_DATA
    char fname [128];
#endif

    // Number of iterations per frame:
    // (This is the way to change how often data is written)
    int iters = 1;

    for(i=0;i<iters;i++) {
	//fprintf(stderr,"%citeration %i   ",13,i+1);
	// The last parameter is extra data for the generic integrator, and should
	// be (void*)p for regular O( n^2 ) gravity, and (void*)ot for the 
	// octree method.  
#ifdef OCTREE_GRAVITY
	p->integrate( p->x, p->gravity, 0.0, p->n*6, p->dt, (void*)ot );
#else
	p->integrate( p->x, p->gravity, 0.0, p->n*6, p->dt, (void*)p );
#endif

	p->t += p->dt;

	// Clip the particle locations, but let the velocities remain constant...
	//bound_particles( p, 15.0 );
    }

#ifdef SAVE_DATA
    create_filename( fname, "../dat/data_",frame,6,".dat" );
    //fprintf(stderr,"   Writing data to `%s'.\n",fname);
    write_particles( p, fname );
#endif

}
