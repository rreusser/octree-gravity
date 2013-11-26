#include <stdio.h>
#include <math.h>
#include <string.h>

#include "common.h"
#include "particles.h"
#include "regular_gravity.h"


// Calculate the gravitational attraction between particles based on 
// O(n^2) pairing of all particles.  Simple naive (but accurate) approach.

void regular_gravity( real t, real* y, int n, real* yp, void* data ) {

    int i,j,i6,j6;
    real r,r2,r3,dx,dy,dz,Fx,Fy,Fz;
    particles* p = (particles*)data;
    real* m = p->m;
    real G = p->G;

    // Initialize y'(t):
    memset(yp,0,sizeof(real)*n);

    // Set up dx/dt = v
    for(i=0;i<p->n;i++) {
	i6=i*6;
	yp[i6  ]=y[i6+3];
	yp[i6+1]=y[i6+4];
	yp[i6+2]=y[i6+5];
    }

    // Loop through HALF of the particle pairs, applying gravitational
    // force to both at the same time:
    for(i=0;i<p->n;i++) {
	i6=6*i;
	for(j=0;j<i;j++) {
	    j6=6*j;

	    // Radius vector:
	    dx = y[i6  ]-y[j6  ];
	    dy = y[i6+1]-y[j6+1];
	    dz = y[i6+2]-y[j6+2];
	    
	    r2 = dx*dx+dy*dy+dz*dz;
	    r = sqrt(r2);

	    // Add in a fudge-factor to prevent div by small numbers
	    // Then you don't get particles slingshotting to infinity
	    // because of imprecision in the timestepping alone.
	    r3 = 1.0/MAX(r*r2,0.00001);

	    // Calculate force in N/kg^2 (save some 
	    // math since we'll just divide it out again)
	    Fx = G*dx*r3;
	    Fy = G*dy*r3;
	    Fz = G*dz*r3;

	    // Accelerations:
	    // Particle i:
	    yp[i6+3] -= Fx*m[j];
	    yp[i6+4] -= Fy*m[j];
	    yp[i6+5] -= Fz*m[j];

	    // Particle j:
	    yp[j6+3] += Fx*m[i];
	    yp[j6+4] += Fy*m[i];
	    yp[j6+5] += Fz*m[i];
	}
    }
}

