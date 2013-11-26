#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "common.h"
#include "particles.h"
#include "point.h"
#include "octree_gravity.h"

// THIS is the factor that controls how far we must descend the tree before
// we stop and just use the center of mass of the rest.
// HUGE impact on speed and accuracy.
real bh_factor = 0.25;

int nnodes=0, nchild=0, npoints=0;
// Calculate the gravitational attraction between particles based on an
// O(n log n) octree data structure.

// Quick lookup array for octree nodes - I know there are more efficient ways...
int id[2][2][2] = {{{0,1},{2,3}},{{4,5},{6,7}}};

// Create a new top-level octree structure
octree* alloc_octree( int n ) {
    octree* ot = malloc(sizeof(octree));
    ot->p = alloc_particles( n );
    ot->index = malloc(sizeof(ot_node*)*n);
    ot->n = n;
    return ot;
}

// Create a new top-level octree structure
void free_octree( octree* ot ) {
    if(ot->root) {
	free_ot_node( ot->root );
	ot->root=NULL;
    }
}

// Create the individual nodes
ot_node* alloc_ot_node() {
    nnodes++;
    nchild+=8;
    ot_node* otn = malloc(sizeof(ot_node));
    otn->children = calloc(8,sizeof(ot_node*));
    otn->pt = malloc(sizeof(point));
	npoints++;
    otn->pt->m = -1.0;
    otn->hasPt = 0;
    return otn;
}

// Free a node
void free_ot_node( ot_node* otn ) {
    int i;
    if(otn->pt != NULL) {
	free(otn->pt);
	npoints--;
    }
    for(i=0;i<8;i++) {
	if(otn->children[i]) {
	    free_ot_node(otn->children[i]);
	}
    }
    free(otn->children);
    nchild-=8;
    free(otn);
    nnodes--;
}

// Locate the bounds - put a *cube* around the outermost points, not a retangular solid
void get_bounds( octree* ot, particles* p ) {
    int i,i6;
    real xmin, xmax, ymin, ymax, zmin, zmax;
    real dx, dy, dz;
    xmin = ymin = zmin = -log(0);
    xmax = ymax = zmax = log(0);
    for(i=0,i6=0;i<p->n;i++) {
	i6=i*6;
	xmin = MIN(xmin,p->x[i6  ]);
	xmax = MAX(xmax,p->x[i6  ]);
	ymin = MIN(ymin,p->x[i6+1]);
	ymax = MAX(ymax,p->x[i6+1]);
	zmin = MIN(zmin,p->x[i6+2]);
	zmax = MAX(zmax,p->x[i6+2]);
    }
    dx = xmax-xmin;
    dy = ymax-ymin;
    dz = zmax-zmin;
    ot->center.x = 0.5*(xmin+xmax);
    ot->center.y = 0.5*(ymin+ymax);
    ot->center.z = 0.5*(zmin+zmax);
    ot->dx = (1.0+1.0e-10)*MAX(MAX(dx,dy),dz);
    //ot->center.y += ot->dx*0.05;
    //ot->dx *= 1.1;
}

// Self-explanatory - insert point masses, one by one
void build_octree_from_particles( octree* ot, particles* p ) {
    get_bounds( ot, p );
    int i;
    for(i=0;i<p->n;i++) {
	insert_point_mass( ot, p->x[6*i],p->x[6*i+1],p->x[6*i+2],p->m[i], i );
    }
}

// Locate and index the leaf nodes into a single array--
// It sounds like a lot of work, but it's significantly
// less than summing forces, so it's worth the trouble
// for simplicity
void build_index( octree* ot, particles* p ) {
    int i;
    for(i=0;i<p->n;i++) {
	ot->index[i] = locate_ot_node( ot->root, p->x[6*i], p->x[6*i+1], p->x[6*i+2], ot->dx*0.5, ot->center );
    }
}

// Debug the index to make sure the points have been inserted correctly
void debug_index( octree* ot, particles* p ) {
    int i;
    fprintf(stderr,"\nIndex:\n");
    for(i=0;i<p->n;i++) {
	fprintf(stderr,"  tried to insert (%x): %f, %f, %f (%f).  Ended up with %f, %f, %f (%f)\n",(unsigned int)ot->index[i],p->x[6*i],p->x[6*i+1],p->x[6*i+2],p->m[i],ot->index[i]->pt->x,ot->index[i]->pt->y,ot->index[i]->pt->z,ot->index[i]->pt->m);
    }
}


// Search for a point
ot_node* locate_ot_node( ot_node* otn, real x, real y, real z, real dx, point center ) {
    int ltx, lty, ltz;
    ot_node* result, *child;
    dx*=0.5;
    ltx = x > center.x;
    lty = y > center.y;
    ltz = z > center.z;
    center.x -= (1-2*ltx)*dx;
    center.y -= (1-2*lty)*dx;
    center.z -= (1-2*ltz)*dx;
    child=otn->children[id[ltx][lty][ltz]];
    if( child == NULL ) {
	result = otn;
	otn->pt->x = x;
	otn->pt->y = y;
	otn->pt->z = z;
    } else {
	result = locate_ot_node( child, x, y, z, dx, center );
    }
    return result;
}

// Insert a point mass into the tree
void insert_point_mass( octree* ot, real x, real y, real z, real m, int ind ) {
    point insert;
    insert.x = x;
    insert.y = y;
    insert.z = z;
    insert.m = m;
    insert_point_mass_node( &(ot->root), &(ot->root), ot->center, &insert, ot->dx*0.5 );
    ot->root->parent = NULL;
}

// *** UGLY *** function to insert point masses, but  it's the data structure that needs
// optimizing, not so much this function.
void insert_point_mass_node( ot_node** otn, ot_node** parent, point center, point* insert, real dx ) {
    point center2;
    int ltx,lty,ltz;
    ot_node** target;
    if(*otn==NULL) {
	// Create a new node if the current node is empty
	*otn = alloc_ot_node();
	(*otn)->pt->x = insert->x;
	(*otn)->pt->y = insert->y;
	(*otn)->pt->z = insert->z;
	(*otn)->pt->m = insert->m;
	(*otn)->parent = *parent;
	(*otn)->dx = dx;
	(*otn)->hasPt = 1;
    } else {
	// Else descend according to which octant it falls in
	dx*=0.5;
	center2 = center;
	ltx = insert->x > center.x;
	lty = insert->y > center.y;
	ltz = insert->z > center.z;
	center.x -= (1-2*ltx)*dx;
	center.y -= (1-2*lty)*dx;
	center.z -= (1-2*ltz)*dx;
	target = &((*otn)->children[id[ltx][lty][ltz]]);
	insert_point_mass_node( target, otn, center, insert, dx );

	// Then we need to fix the point the used to occupy this node
	// by also locating a new octant
	if((*otn)->hasPt == 1) {
	    ltx = (*otn)->pt->x > center2.x;
	    lty = (*otn)->pt->y > center2.y;
	    ltz = (*otn)->pt->z > center2.z;
	    center2.x -= (1-2*ltx)*dx;
	    center2.y -= (1-2*lty)*dx;
	    center2.z -= (1-2*ltz)*dx;
	    target = &((*otn)->children[id[ltx][lty][ltz]]);
	    insert_point_mass_node( target, otn, center2, (*otn)->pt, dx );
	    (*otn)->hasPt = 0;
	}
    }
}

// DEBUG FUNCTIONS
void traverse_debug(octree* ot) {
    fprintf(stderr,"\nTraverse debug");
    traverse_debug_ot_node( ot->root );
}

void traverse_debug_ot_node( ot_node* otn ) {
    int i,nch=0;
    if( otn != NULL ) {
	dbgp("\nNon-NULL child",(void*)(unsigned int)otn);
	for(i=0;i<8;i++) {
	    fprintf(stderr,"%i",otn->children[i]!=NULL);
	    if( otn->children[i]!=NULL ) {
		nch++;
	    }
	}
	fprintf(stderr,"0x%x has %i children\n",(unsigned int)otn,nch);
	for(i=0;i<8;i++) {
	    traverse_debug_ot_node( otn->children[i] );
	}
    }
}

// Recursively calculate the center of mass
point ot_node_calculate_masses( ot_node* otn ) {
    point cm, ccm;
    int i,hasChildren=0;
    // Add up the CM recursively through all the children
    cm.x=cm.y=cm.z=cm.m=0.0;
    for(i=0;i<8;i++) {
	if(otn->children[i] != NULL) {
	    hasChildren++;
	    ccm = ot_node_calculate_masses( otn->children[i] );
	    cm.x += ccm.x * ccm.m;
	    cm.y += ccm.y * ccm.m;
	    cm.z += ccm.z * ccm.m;
	    cm.m += ccm.m;
	}
    }
    // Find out if it's a leaf node, then just add the single-particle mass
    if(hasChildren==0) {
	// Then we're at a leaf
	cm.x = otn->pt->x;
	cm.y = otn->pt->y;
	cm.z = otn->pt->z;
	cm.m = otn->pt->m;
    } else {
	cm.x /= cm.m;
	cm.y /= cm.m;
	cm.z /= cm.m;
	otn->pt->x = cm.x;
	otn->pt->y = cm.y;
	otn->pt->z = cm.z;
	otn->pt->m = cm.m;
    }
    return cm;
}

// Sum the masses recursively into a hierarchy of centers-of-mass in each node
void octree_calculate_masses( octree* ot ) {
    point cm;
    cm = ot_node_calculate_masses( ot->root );
}

// Integration routine to return the derivative - THE MOST EXPENSIVE ROUTINE!!!
void octree_gravity( real t, real* y, int n, real* yp, void* data ) {
    int i,i6;
    point F;
    octree* ot = (octree*)data;
    particles* p = (particles*)ot->p;

    // Free the old octree, and build a new one:
    free_octree( ot );
    memcpy(p->x,y,sizeof(real)*p->n*6);
    build_octree_from_particles( ot, p );

    build_index( ot, p );
    octree_calculate_masses( ot );

    // Initialize y'(t):
    memset(yp,0,sizeof(real)*n);

    // Set up dx/dt = v
    for(i=0;i<p->n;i++) {
	i6=i*6;
	yp[i6  ]=y[i6+3];
	yp[i6+1]=y[i6+4];
	yp[i6+2]=y[i6+5];

	//  *** THE ONLY EXPENSIVE PART!!! ***
	F = octree_add_forces( ot, p, i );

	yp[i6+3] = F.x;
	yp[i6+4] = F.y;
	yp[i6+5] = F.z;
    }
}



// Add up the forces acting on a single particle i
point octree_add_forces( octree* ot, particles* p, int i ) {
    point F;
    F = octree_add_node_forces( ot->index[i], ot->index[i], NULL, p );
    return F;
}

// Descend the tree until the rest of the particles are insiginificant
// enough that we can just calculate a force from the center of mass
point octree_descend_node_forces( ot_node* target, ot_node* otn, particles* p ) {
    point F, Fp;
    F.x=F.y=F.z=F.m=0.0;
    int i,nch=0,addPt=0;;

    // Check whether we need to descend a level and include all children,
    // or whether we can just calculate the force from the CM
    if( SQR(otn->dx) / 
	(SQR(target->pt->x-otn->pt->x) +
	 SQR(target->pt->y-otn->pt->y) +
	 SQR(target->pt->z-otn->pt->z)) > bh_factor) {
	for(i=0;i<8;i++) {
	    if(otn->children[i]!=NULL) {
		nch++;
		Fp = octree_descend_node_forces( target, otn->children[i], p );
		F.x += Fp.x;
		F.y += Fp.y;
		F.z += Fp.z;
	    }
	}
	if(nch==0) { addPt=1; }
    } else { addPt=1; }

    if(addPt) {
	// FINALLY, calculate a force by Newton's Law of Gravitation
	real dx, dy, dz, r, r2, r3, mu;

	dx = otn->pt->x - target->pt->x;
	dy = otn->pt->y - target->pt->y;
	dz = otn->pt->z - target->pt->z;

	r2 = dx*dx+dy*dy+dz*dz;
	r = sqrt(r2);
	// Prevent div by zero:
	r3 = 1.0/MAX(r2*r,0.00001);
    
	mu = p->G*otn->pt->m;
	F.x = mu*dx*r3;
	F.y = mu*dy*r3;
	F.z = mu*dz*r3;
    }
    return F;
}

// Ascend the tree until we reach the top, after which we can descend
// and add the forces up - need to do it this way so a force does not act on itself
point octree_add_node_forces( ot_node* target, ot_node* otn, ot_node* child, particles* p ) {
    point F, Fp;
    F.x = F.y = F.z = F.m = 0.0;
    int i;
    // Unless we're at the top, call this function recursively for
    // the parent node
    if(otn->parent != NULL) {
	Fp = octree_add_node_forces( target, otn->parent, otn, p );
	F.x += Fp.x;
	F.y += Fp.y;
	F.z += Fp.z;
    }
    // Descend the rest of the nodes on the way up, as long as that node
    // isn't the child we came from
    for(i=0;i<8;i++) {
	if(otn->children[i] != NULL && otn->children[i] != child ) {
	    Fp = octree_descend_node_forces( target, otn->children[i], p );
	    F.x += Fp.x;
	    F.y += Fp.y;
	    F.z += Fp.z;
	}
    }
    return F;
}
