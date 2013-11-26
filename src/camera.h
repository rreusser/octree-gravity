#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "vector3f.h"
#include "func.h"
#include "GL/glut.h"

#define VIEW_ROTATE 0
#define VIEW_ZOOM 1
#define VIEW_FIELD 2

struct camera {
    vector3f *location, *location0;
    vector3f *lookAt, *lookAt0;
    vector3f *sky, *sky0;
    vector3f* camx;
    vector3f* camy;
    vector3f* camz;
    float angle, angle0;
    float theta, theta0;
    float dist,  dist0;
    float phi,   phi0;
    float close, far;
    int width, height;
    float ratio;
};

//Allocate a new camera and return its pointer
struct camera* NewCamera();

//Free the camera
void FreeCamera(struct camera* cam);

//Update the camera when the window shape changes
void CameraReshapeWindow(struct camera* cam, int winWidth, int winHeight);

//Set clipping planes
void CameraLimits(struct camera* cam, float close, float far);

//Tell the camera information to OpenGL
void MakeOGLCamera(struct camera* cam);

//Position camera according to vector location and target
void CameraPositionVecVec(struct camera* cam, vector3f* location, vector3f* lookAt);

//Position camera according to XYZ coordinates for location and target
void CameraPositionXYZXYZ(struct camera* cam, float camx, float camy, float camz, float lax, float lay, float laz);

//Position camera about vector target in spherical coordinates, (r, phi, theta)
void CameraPositionRPTVec(struct camera* cam, float dist, float phi, float theta, vector3f* lookAt);

//Position camera about XYZ coordinates of target in spherical coordinates, (r, phi, theta)
void CameraPositionRPTXYZ(struct camera* cam, float dist, float phi, float theta, float lax, float lay, float laz);

//Store the old vew parameters
void CameraRemember(struct camera* cam);

//Recalculate position of camera without changing it
void CameraReposition(struct camera* cam);

//Point at a new target
void CameraTarget(struct camera* cam, float x, float y, float z);

//Calculate the camera xyz
void CameraCalculateAxes(struct camera* cam);
#endif
