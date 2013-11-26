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
#include "octree_draw.h"

extern real bh_factor;

// Draw the tree structure
void draw_children( ot_node* otn ) {
    int i;
    if( otn != NULL ) {
	if(otn->parent != NULL) {
	    glColor3f( 0.0, 1.0, 1.0 );
	    glBegin( GL_LINES );
		glVertex3f( otn->parent->pt->x, otn->parent->pt->y, otn->parent->pt->z );
		glVertex3f( otn->pt->x, otn->pt->y, otn->pt->z );
	    glEnd();
	}
	glColor3f( 0.1+0.02/otn->dx, 0.1+0.02/otn->dx, 0.1+0.02/otn->dx );
	//glBegin( GL_LINES );
	    //glVertex3f( otn->pt->x-otn->dx, otn->pt->y, otn->pt->z );
	    //glVertex3f( otn->pt->x+otn->dx, otn->pt->y, otn->pt->z );
	    //glVertex3f( otn->pt->x, otn->pt->y-otn->dx, otn->pt->z );
	    //glVertex3f( otn->pt->x, otn->pt->y+otn->dx, otn->pt->z );
	    //glVertex3f( otn->pt->x, otn->pt->y, otn->pt->z-otn->dx );
	    //glVertex3f( otn->pt->x, otn->pt->y, otn->pt->z+otn->dx );
	//glEnd();
	for(i=0;i<8;i++) {
	    draw_children( otn->children[i] );
	}
    }
}

// Draw the cubes:
void draw_octree_node( ot_node* otn, real dx, point center, int num ) {
    int i,j;
    if(otn!=NULL) {
	glBegin( GL_LINES );
	    for(i=-1;i<=1;i+=2) {
		for(j=-1;j<=1;j+=2) {
		    glVertex3f( center.x-dx, center.y+i*dx, center.z+j*dx );
		    glVertex3f( center.x+dx, center.y+i*dx, center.z+j*dx );
		    glVertex3f( center.x+i*dx, center.y-dx, center.z+j*dx );
		    glVertex3f( center.x+i*dx, center.y+dx, center.z+j*dx );
		    glVertex3f( center.x+i*dx, center.y+j*dx, center.z-dx );
		    glVertex3f( center.x+i*dx, center.y+j*dx, center.z+dx );
		}
	    }
	glEnd();
	dx*=0.5;
	draw_octree_node( otn->children[0], dx, add(center,-dx,-dx,-dx), 0);
	draw_octree_node( otn->children[1], dx, add(center,-dx,-dx,dx), 1);
	draw_octree_node( otn->children[2], dx, add(center,-dx,dx,-dx), 2);
	draw_octree_node( otn->children[3], dx, add(center,-dx,dx,dx), 3);
	draw_octree_node( otn->children[4], dx, add(center,dx,-dx,-dx), 4);
	draw_octree_node( otn->children[5], dx, add(center,dx,-dx,dx), 5);
	draw_octree_node( otn->children[6], dx, add(center,dx,dx,-dx), 6);
	draw_octree_node( otn->children[7], dx, add(center,dx,dx,dx), 7);
    }
}
void draw_octree( octree* ot ) {
    draw_octree_node( ot->root, ot->dx*0.5, ot->center, 0 );
}

// SAME AS octree_gravity.c, BUT DRAW AS WE GO...
point octree_add_forces_draw( octree* ot, particles* p, int i ) {
    point F;
    F = octree_add_node_forces_draw( ot->index[i], ot->index[i], NULL, p );
    return F;
}

point octree_descend_node_forces_draw( ot_node* target, ot_node* otn, particles* p ) {
    point F, Fp;
    F.x=F.y=F.z=F.m=0.0;
    int i,nch=0,addPt=0;;
    glColor3f( 1.0, 0.0, 0.0 );
    glBegin( GL_LINES );
	glVertex3f( otn->parent->pt->x, otn->parent->pt->y, otn->parent->pt->z );
	glVertex3f( otn->pt->x, otn->pt->y, otn->pt->z );
    glEnd();
    //real dist = sqrt(SQR(target->pt->x-otn->pt->x) + SQR(target->pt->y-otn->pt->y) + SQR(target->pt->z-otn->pt->z));
    //real dia = otn->dx;
    //fprintf(stderr,"dia=%f, dist=%f, rat=%f",dia,dist,dia/dist);
    //if(dia/dist > bh_factor ) {
	//fprintf(stderr,"  subdiv=YES\n");

    if( SQR(otn->dx) / 
	(SQR(target->pt->x-otn->pt->x) +
	 SQR(target->pt->y-otn->pt->y) +
	 SQR(target->pt->z-otn->pt->z)) > bh_factor) {
	for(i=0;i<8;i++) {
	    if(otn->children[i]!=NULL) {
		nch++;
		Fp = octree_descend_node_forces_draw( target, otn->children[i], p );
		F.x += Fp.x;
		F.y += Fp.y;
		F.z += Fp.z;
	    }
	}
	if(nch==0) { addPt=1; }
    } else { addPt=1; 
	//fprintf(stderr,"  subdiv=NO\n");
    }

    if(addPt) {
	real dx, dy, dz, r, r2, r3, mu;

	dx = otn->pt->x - target->pt->x;
	dy = otn->pt->y - target->pt->y;
	dz = otn->pt->z - target->pt->z;

	r2 = dx*dx+dy*dy+dz*dz;
	r = sqrt(r2);
	r3 = 1.0/MAX(r2*r,0.00001);
    
	mu = p->G*otn->pt->m;
	F.x = mu*dx*r3;
	F.y = mu*dy*r3;
	F.z = mu*dz*r3;
    }
    return F;
}

point octree_add_node_forces_draw( ot_node* target, ot_node* otn, ot_node* child, particles* p ) {
    point F, Fp;
    F.x = F.y = F.z = F.m = 0.0;
    int i;
    if(otn->parent != NULL) {
	glColor3f( 0.0, 1.0, 0.0 );
	glBegin( GL_LINES );
	    glVertex3f( otn->parent->pt->x, otn->parent->pt->y, otn->parent->pt->z );
	    glVertex3f( otn->pt->x, otn->pt->y, otn->pt->z );
	glEnd();
	Fp = octree_add_node_forces_draw( target, otn->parent, otn, p );
	F.x += Fp.x;
	F.y += Fp.y;
	F.z += Fp.z;
    }
    for(i=0;i<8;i++) {
	if(otn->children[i] != NULL && otn->children[i] != child ) {
	    Fp = octree_descend_node_forces_draw( target, otn->children[i], p );
	    F.x += Fp.x;
	    F.y += Fp.y;
	    F.z += Fp.z;
	}
    }
    return F;
}

