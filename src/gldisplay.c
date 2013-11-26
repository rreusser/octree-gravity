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
#include "gldisplay.h"

extern int simulationOn;
extern struct camera* cam;
extern particles *p;
extern octree* ot;

int sample=0;
int drawOctreeOn = 0;
int drawTreeOn = 0;
int sampleForceOn = 0;
int nextdisp = 1; //disable simulation step when anything but the timer calls disp();
int axisOn = 1;
int fps=1000; //frames per second
int frame=0;

//MAIN DISPLAY FUNCTION:
//	- draw axes
//	- draw grid
//	- draw curve
//	- draw curvature at c0
void glDisplay() {
    int i;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(axisOn) {
	glBegin( GL_LINES );
	float eps = 0.501;
	//DRAW GRID
	for(i=-10; i<=10; i+=2) {
	    glColor3f(0.2,0.2,0.2);
	    glVertex3f(i/10.0,-eps,1.0);
	    glVertex3f(i/10.0,-eps,-1.0);
	    glVertex3f(-1.0,-eps,i/10.0);
	    glVertex3f(1.0,-eps,i/10.0);
	    glColor3f(1.0,1.0,1.0);
	    glVertex3f(i/10.0,-eps,0.02);
	    glVertex3f(i/10.0,-eps,-0.02);
	    glVertex3f(-0.02,-eps,i/10.0);
	    glVertex3f(0.02,-eps,i/10.0);
	}
	//DRAW AXES
	glColor3f(1.0,1.0,1.0);
	glVertex3f(-1.0,-eps,0.0);
	glVertex3f(1.0,-eps,0.0);
	glVertex3f(0.0,-eps,-1.0);
	glVertex3f(0.0,-eps,1.0);
	glEnd();
    }

    if(simulationOn==1 && nextdisp==1) {

	iterate();

	frame++;
	nextdisp=0;
    }


    // Draw sample force tree
    if(sampleForceOn) {
	glColor3f(0.0,1.0,0.5);
	octree_add_forces_draw( ot, p, sample );
    }

    // Should we draw the octree cubes?
    if(drawOctreeOn) {
	glColor3f(0.0,0.0,1.0);
	draw_octree( ot );
    }
    // Draw the full center of mass tree structure:
    if(drawTreeOn) {
	glColor3f(0.0,1.0,1.0);
	draw_children( ot->root );
    }
    // Or just draw the plain particles
    glColor3f(1.0,1.0,1.0);
    draw_particles( p );

    //Once everything is drawn, put it on the screen
    glutSwapBuffers();
}

void glRedisplay(int value) {
    if(value==1) {
	glDisplay();
    }
    if(simulationOn==1) {
	glutTimerFunc(1000/fps,glRedisplay,1);
	nextdisp=1;
    }
}

//SET UP THE CAMERA LOCATION
void glReView() { MakeOGLCamera(cam); }

//UPDATE THE WINDOW SIZE
void glReshape(int w, int h) {
    CameraReshapeWindow(cam,w,h);
    glReView();
}


