#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "common.h"
#include "particles.h"

void init_particles( particles* p ) {
    if(0) {
	// Random particles, good for debugging octree, but not so interesting:
	randomize_particles( p, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 0.0 );
	randomize_masses( p, 1.0, 2.0 );
    } else {
	// Else one/two galaxies with large masses at the center
	int i;
	real rad,ht;
	real vscale;
	// Velocity at which they collide:
	real xv = 5.0*0;
	real zvL = 3.0*0;
	real zvR = 3.0*0;
	real x0L = -1.0*0;
	real x0R = 1.0*0;

	for(i=0;i<p->n;i++) {
	    p->m[i] = 1.0/250.0*10.0;

	    // Calculate a radius:
	    rad = drand48();
	    // Clip to make sure it's not too close to the center mass
	    rad=MAX(rad,0.05)*0.5;
	    // Calculate a (small) height
	    ht = SQR(drand48()-0.5)*exp(-SQR(rad)*35)*0.10;
	    // Radial velocity scaling:
	    vscale = (2.0 * pow(rad,-0.5) + rad*2.0)*15.0;
	    // Incline of rightmost galaxy

#ifdef TWO_GALAXIES
	    // Right, inclined galaxy
	    real incline = 45.0 * M_PI/180.0;
	    if(i<p->n/2) {
		p->x[6*i  ] = cos(M_PI*4.0*i/p->n)*rad;
		p->x[6*i+1] = ht*cos(incline) - sin(M_PI*4.0*i/p->n)*rad*sin(incline);
		p->x[6*i+2] = sin(M_PI*4.0*i/p->n)*rad*cos(incline) + ht*sin(incline);
		p->x[6*i+3] = -sin(M_PI*4.0*i/p->n)*vscale;
		p->x[6*i+4] = (-cos(M_PI*4.0*i/p->n)*vscale)*sin(incline);
		p->x[6*i+5] = ( cos(M_PI*4.0*i/p->n)*vscale)*cos(incline);
		p->x[6*i] += x0R;
		p->x[6*i+3] -= xv;
		p->x[6*i+5] -= zvR;
	    } else {
#endif
	    // Left galaxy
		p->x[6*i  ] = cos(M_PI*4.0*i/p->n)*rad;
		p->x[6*i+1] = ht;
		p->x[6*i+2] = sin(M_PI*4.0*i/p->n)*rad;
		p->x[6*i+3] = -sin(M_PI*4.0*i/p->n)*vscale;
		p->x[6*i+4] = 0.0;
		p->x[6*i+5] = cos(M_PI*4.0*i/p->n)*vscale;
		p->x[6*i] += x0L;
		p->x[6*i+3] += xv;
		p->x[6*i+5] += zvL;
#ifdef TWO_GALAXIES
	    }
#endif
	}
	// Center mass for right galaxy
	p->x[0] = x0R;
	p->x[1] = 0.0;
	p->x[2] = 0.0;
	p->x[3] = -xv*0.0;
	p->x[4] = 0.0;
	p->x[5] = -zvR*0.0;
	p->m[0] = 1000.0;
	// Center mass for left galaxy
	//p->x[p->n/2*6] = x0L;
	//p->x[p->n/2*6+1] = 0.0;
	//p->x[p->n/2*6+2] = 0.0;
	//p->x[p->n/2*6+3] = xv;
	//p->x[p->n/2*6+4] = 0.0;
	//p->x[p->n/2*6+5] = zvL;
	//p->m[p->n/2] = 100.0;
    }
}
