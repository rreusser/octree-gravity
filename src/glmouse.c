#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include "camera.h"
#include "gldisplay.h"
#include "glmouse.h"


int mouseDisabled = 0; //make sure the mouse doesn't update too often
double mpx = 0.0; //current location of mouse
double mpy = 0.0;
double mpx0 = 0.0; //location of mouse when clicked
double mpy0 = 0.0; 
extern struct camera* cam;
int clickMode;



//CALLED ON MOUSE CLICK EVENT
void glMouseFunc(int button, int state, int x, int y) {

    //Remember current reference state of view
    CameraRemember(cam);

    switch(state) {
	case GLUT_DOWN:
	    mpx0 = (float)x/(float)cam->width; //calculate mouse position based on x,y function input
	    mpy0 = 1.0-(float)y/(float)cam->height;

	    switch(button) {
	    case GLUT_LEFT_BUTTON:
		clickMode = VIEW_ROTATE;
	    break;
	    case GLUT_MIDDLE_BUTTON:
		clickMode = VIEW_ZOOM;
	    break;
	    case GLUT_RIGHT_BUTTON:
		clickMode = VIEW_TRANSLATE;
		CameraCalculateAxes(cam);
	    break;
	    }
	break;
	case GLUT_UP:
	break;
    }
}

void glReEnableMouse(int value) {
    mouseDisabled=0;
}

void glMotionFunc(int x, int y) {
    if(mouseDisabled==0) { //if the mouse is disabled, then this function has been called too recently
	vector3f temp1,temp2,temp3,temp4;
	float dx,dy;
	mpx = (float)x/(float)cam->width;
	mpy = 1.0-(float)y/(float)cam->height;
	dx = mpx-mpx0;
	dy = mpy-mpy0;
	switch(clickMode) {
	    case VIEW_ROTATE:
		//use the point recorded when the mouse was clicked
		//versus the current point when held down to decided
		//how much to rotate the view around and up or down
		cam->theta = cam->theta0 + dx*360.0;
		cam->phi = cam->phi0 + dy*180.0;
		CameraReposition(cam);
		glReView();
	    break;
	    case VIEW_ZOOM:
		//use the same two points to calculate a dy which tells
		//how much to zoom in or out - exponentially
		cam->dist = cam->dist0*exp(dy*4.0);
		CameraReposition(cam);
		glReView();
	    break;
	    case VIEW_TRANSLATE:
		//If the view should be translated, then use the screen coordinate
		//system to move the camera in the plane of the view
		vfCopy(cam->location,vfSub(&temp1,cam->location0,vfAdd(&temp2,
		    vffMult(&temp3,cam->camx,dx*cam->dist),vffMult(&temp4,cam->camy,dy*cam->dist))));
		vfCopy(cam->lookAt,vfSub(&temp1,cam->lookAt0,vfAdd(&temp2,
		    vffMult(&temp3,cam->camx,dx*cam->dist),vffMult(&temp4,cam->camy,dy*cam->dist))));
		glReView();
	    break;
	}
	glDisplay();
	mouseDisabled=1;
	glutTimerFunc(1000/60,glReEnableMouse,1); //reactivate this function in a little bit
    }
}
