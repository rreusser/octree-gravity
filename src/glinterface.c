#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include "camera.h"
#include "gldisplay.h"
#include "glkeys.h"
#include "glmouse.h"
#include "glinterface.h"

int win;
int width; //window width and height
int height;
int full=0; //fullscreen?
int simulationOn=0;
struct camera* cam;

void init_glut( int argc, char **argv ) {

    //INITIALIZE CAMERA
    cam = NewCamera();
    CameraPositionRPTXYZ( cam, 2.5, 20.0, 90.0, 0.0, 0.0, 0.0 );
    
    //INITIALIZE GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA );
    glutInitWindowSize(640,480);
    glutInitWindowPosition(0,0);
    win = glutCreateWindow("Window");

    fprintf(stderr,"\n\n*********** N-Body Gravity Example  ***********\n");
    fprintf(stderr,    "|                                             |\n");
    fprintf(stderr,    "|    by Ricky Reusser  (rsreusser@gmail.com)  |\n");
    fprintf(stderr,    "|    Enjoy, just let me know if it's useful!  |\n");
    fprintf(stderr,    "|                                             |\n");
    fprintf(stderr,    "***********************************************\n");
    fprintf(stderr,"\nUse the three mouse buttons to rotate, pan, and zoom.\n\n");

    fprintf(stderr,"spacebar to start/stop calculation\n");
    fprintf(stderr,"=/- to inc/dec timestep\n\n");

    fprintf(stderr,"t to toggle tree display\n");
    fprintf(stderr,"o to toggle octree display\n");
    fprintf(stderr,"s for sample force tree (only once simulation has\n");
    fprintf(stderr,"  been started, or you can expect a bus error!\n\n");
    fprintf(stderr,"a to toggle axis\n");

    fprintf(stderr,"left/right arrow to sample force at different points\n");
    fprintf(stderr,"up/down arrow to inc/dec Barnes Hut octree threshold\n\n");
    fprintf(stderr,"r to restart\n");
    fprintf(stderr,"q to quit\n");
    fprintf(stderr,"\nFor more info, read the source!!!\n\n\n");

    //GLUT CALLBACK FUNCTIONS
    glutDisplayFunc(glDisplay);
    glutReshapeFunc(glReshape);
    glutKeyboardFunc(glKeyboard);
    glutMouseFunc(glMouseFunc);
    glutMotionFunc(glMotionFunc);
    glutSpecialFunc(glSpecialKeys);
    
    //MAKE THE LINES SMOOTH
    if(1) {
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glLineWidth( 1.0 );
	glPointSize( 1.0 );
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth( 1.0f );
    }
}

