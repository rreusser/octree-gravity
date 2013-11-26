#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include "camera.h"
#include "gldisplay.h"
#include "common.h"
#include "particles.h"
#include "point.h"
#include "rk.h"
#include "octree_gravity.h"
#include "initial_conditions.h"
#include "main.h"
#include "glkeys.h"

extern real bh_factor;
extern int sampleForceOn;
extern int sample;
extern int full;
extern int simulationOn;
extern int axisOn;
extern int win;
extern int drawOctreeOn;
extern int drawTreeOn;
extern int rk_number;
extern particles *p;
extern octree* ot;

//KEYBOARD FUNCTION
void glKeyboard(unsigned char key, int x, int y) {
    switch(key) {
	case '-':
	    p->dt /= 1.1;
	break;
	case '=':
	    p->dt *= 1.1;
	break;
	case '0': //toggle fullscreen size
	    if(full==1) {
		glutReshapeWindow(500,500);
		glutPositionWindow(100,100);
	    } else {
		glutReshapeWindow(1024,768);
		glutPositionWindow(0,-10);
	    }
	    full=!full;
	break; 
	case 'i':
	    switch(rk_number) {
		case 2: rk_number = 4; p->integrate = rk4; break;
		case 4: rk_number = 8; p->integrate = rk8; break;
		case 8: rk_number = 2; p->integrate = rk2; break;
	    }
	    fprintf(stderr,"Using RK%i integrator\n",rk_number);
	break;
	case 'r':
	    simulationOn=0;
	    fprintf(stderr,"p->n = %i\n",p->n);
	    init_particles( p );
	    free_octree( ot );
	    build_octree_from_particles( ot, p );
	    get_bounds( ot, p );
	    octree_calculate_masses( ot );
	    glDisplay();
	break;
	case 's':
	    sampleForceOn = 1-sampleForceOn;
	    glDisplay();
	break;
	case ' ':
	    if(simulationOn==0) {
		simulationOn=1;
		glRedisplay(1);
	    } else {
		simulationOn=0;
	    }
	break;
	case 't':
	    drawTreeOn = 1-drawTreeOn;
	    glDisplay();
	break;
	case 'o':
	    drawOctreeOn = 1-drawOctreeOn;
	    glDisplay();
	break;
	case 'a': axisOn = 1-axisOn; glDisplay();		break;
	case ')': glutFullScreen(); full=1; 		break;  //toggle fullscreen
	case 'v': glDisplay(); 				break;  //redisplay
	case 'q': glutDestroyWindow(win); exit(0); 	break;  //quit
    }
}


//CALLBACK FUNCTION TO HANDLE SPECIAL KEYS (arrows, etc)
void glSpecialKeys(int key, int x, int y) {
    switch(key) {
	case GLUT_KEY_RIGHT:
	    sample++;
	    glDisplay();
	break;
	case GLUT_KEY_LEFT:
	    sample--;
	    glDisplay();
	break;
	case GLUT_KEY_DOWN:
	    bh_factor -= 0.05;
	    fprintf(stderr,"Barnes Hut factor = %f\n",(bh_factor));
	    glDisplay();
	break;
	case GLUT_KEY_UP:
	    bh_factor += 0.05;
	    fprintf(stderr,"Barnes Hut factor = %f\n",(bh_factor));
	    glDisplay();
	break;
    }
    sample = MIN(p->n-1,MAX(0,sample));
    glDisplay();
}

