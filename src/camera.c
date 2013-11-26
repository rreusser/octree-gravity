#include "camera.h"

//Allocate a new camera and return its pointer
struct camera* NewCamera(int winWidth, int winHeight) {
    struct camera* cam = malloc(sizeof(struct camera));
    cam->location = newVector3f(0.0,0.0,1.0);
    cam->location0 = newVector3f(0.0,0.0,1.0);
    cam->lookAt = newVector3f(0.0,0.0,0.0);
    cam->lookAt0 = newVector3f(0.0,0.0,0.0);
    cam->sky = newVector3f(0.0,1.0,0.0);
    cam->sky0 = newVector3f(0.0,1.0,0.0);
    cam->camx = newVector3f(0.0,0.0,0.0);
    cam->camy = newVector3f(0.0,0.0,0.0);
    cam->camz = newVector3f(0.0,0.0,0.0);
    cam->phi = 90.0;
    cam->theta = 90.0;
    cam->dist = 1.0;
    cam->angle = 45.0;
    cam->close = 0.01;
    cam->far = 100.0;
    CameraReshapeWindow(cam,winWidth,winHeight);
    return cam;
}

//Free the camera
void FreeCamera(struct camera* cam) {
    free(cam->location);
    free(cam->location0);
    free(cam->lookAt);
    free(cam->lookAt0);
    free(cam->sky);
    free(cam->sky0);
    free(cam);
    cam=NULL;
}

//Update the camera when the window shape changes
void CameraReshapeWindow(struct camera* cam, int winWidth, int winHeight) {
    if(winHeight==0) winHeight=1;
    if(winWidth==0) winWidth=1;
    cam->width = winWidth;
    cam->height = winHeight;
    cam->ratio = (float)winWidth/(float)winHeight;
}

//Set clipping planes
void CameraLimits(struct camera* cam, float close, float far) {
    cam->close = close;
    cam->far = far;
}

//Tell the camera information to OpenGL
void MakeOGLCamera(struct camera* cam) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, cam->width, cam->height);
    gluPerspective(cam->angle,cam->ratio,cam->close, cam->far + cam->dist);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
	cam->location->x,
	cam->location->y,
	cam->location->z,
	cam->lookAt->x,
	cam->lookAt->y,
	cam->lookAt->z,
	cam->sky->x,
	cam->sky->y,
	cam->sky->z
    );
}


//Position camera according to vector location and target
void CameraPositionVecVec(struct camera* cam, vector3f* location, vector3f* lookAt) {
    vfCopy(cam->location, location);
    vfCopy(cam->lookAt, lookAt);
}

//Position camera according to XYZ coordinates for location and target
void CameraPositionXYZXYZ(struct camera* cam, float camx, float camy, float camz, float lax, float lay, float laz) {
    cam->location->x = camx;
    cam->location->y = camy;
    cam->location->z = camz;
    cam->lookAt->x = lax;
    cam->lookAt->y = lay;
    cam->lookAt->z = laz;
}

//Position camera about vector target in spherical coordinates, (r, phi, theta)
void CameraPositionRPTVec(struct camera* cam, float dist, float phi, float theta, vector3f* lookAt) {
    cam->dist = dist;
    cam->phi = phi;
    cam->theta = theta;
    cam->location->x = lookAt->x+cam->dist*cos(radians(theta))*sin(radians(phi));
    cam->location->y = lookAt->y+cam->dist*cos(radians(phi));
    cam->location->z = lookAt->z+cam->dist*sin(radians(theta))*sin(radians(phi));
    vfCopy(cam->lookAt, lookAt);
}

//Position camera about XYZ coordinates of target in spherical coordinates, (r, phi, theta)
void CameraPositionRPTXYZ(struct camera* cam, float dist, float phi, float theta, float lax, float lay, float laz) {
    cam->dist = dist;
    cam->phi = phi;
    cam->theta = theta;
    cam->location->x = lax+cam->dist*cos(radians(theta))*sin(radians(phi));
    cam->location->y = lay+cam->dist*cos(radians(phi));
    cam->location->z = laz+cam->dist*sin(radians(theta))*sin(radians(phi));
    cam->lookAt->x = lax;
    cam->lookAt->y = lay;
    cam->lookAt->z = laz;
}

//Recalculate position of camera without changing it
void CameraReposition(struct camera* cam) {
    cam->location->x = cam->lookAt->x+cam->dist*cos(radians(cam->theta))*sin(radians(cam->phi));
    cam->location->y = cam->lookAt->y+cam->dist*cos(radians(cam->phi));
    cam->location->z = cam->lookAt->z+cam->dist*sin(radians(cam->theta))*sin(radians(cam->phi));
}

//Point at a new target
void CameraTarget(struct camera* cam, float x, float y, float z) {
    cam->lookAt->x = x;
    cam->lookAt->y = y;
    cam->lookAt->z = z;
    CameraReposition(cam);
}

//Store the old vew parameters
void CameraRemember(struct camera* cam) {
    cam->angle0 = cam->angle;
    cam->theta0 = cam->theta;
    cam->dist0 = cam->dist;
    cam->phi0 = cam->phi;
    vfCopy(cam->location0,cam->location);
    vfCopy(cam->lookAt0,cam->lookAt);
    vfCopy(cam->sky0,cam->sky);
}

//Calculate the camera xyz
void CameraCalculateAxes(struct camera* cam) {
    //vfCopy(&sky,cam->sky);
    //vfCopy(&loc,cam->location);
    //vfCopy(&tar,cam->lookAt);
    vfSub(cam->camz,cam->lookAt,cam->location);
    //sky.x = 0.0; sky.y = 1.0; sky.z = 0.0;
    vfCross(cam->camx,cam->camz,cam->sky);
    vfCross(cam->camy,cam->camx,cam->camz);
    vfNormalize(cam->camx);
    vfNormalize(cam->camy);
    vfNormalize(cam->camz);
}

