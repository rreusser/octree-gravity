#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "common.h"
#include "particles.h"
#include "rk.h"
#include "point.h"
#include "initial_conditions.h"
#include "regular_gravity.h"
#include "glinterface.h"
#include "octree_gravity.h"
#include "iterate.h"
#include "main.h"

particles *p;
octree* ot;
int rk_number=4;

int main(int argc, char** argv) {

    int n = 4000;
    p = alloc_particles( n );

    p->dt = 0.0001;
    p->integrate = rk4;
    p->G = 1.0;



// (optionally) defined in common.h
#ifdef OCTREE_GRAVITY

    n = p->n;
    ot = alloc_octree( n );
    //read_particle_data( &p, "../../GalaxyIC/mw_c.dat" );
    //read_particle_data( &(ot->p), "../../GalaxyIC/mw_c.dat" );

    init_particles( p );
    init_particles( ot->p );
    ot->p->G = 1.0;
    p->gravity = octree_gravity;

#else
    init_particles( p );
    p->gravity = regular_gravity;
#endif




#ifdef OPENGL
    init_glut( argc, argv );
    glutMainLoop();
#else
    // Careful, you might not want to leave this running all day
    // if it's writing data!
    while(1) {
	fprintf(stderr,".");
	iterate();
    }
#endif


    return 0;
}
