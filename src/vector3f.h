#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include <stdlib.h>
#include <math.h>

//3-component float vector
typedef struct {
    float x, y, z;
} vector3f;

//ALLOCATE NEW VECTORS
//create a new float vector and return its pointer
vector3f* newVector3f(float x, float y, float z);

//COPY VECTORS
//float: src --> target
vector3f* vfCopy(vector3f* target, vector3f* src);


//FLOAT VECTOR OPERATIONS
//Add two float vectors: A=A+B
vector3f* vfAdd(vector3f* A, vector3f* B, vector3f* C);
vector3f* vfPlusEq(vector3f* A, vector3f* B);

//subtract float vectors: C=A-B
vector3f* vfSub(vector3f* A, vector3f* B, vector3f* C);
vector3f* vfMinusEq(vector3f* A, vector3f* B);

//calculate product element by element: C(i) = A(i)*B(i)
vector3f* vfMult(vector3f* A, vector3f* B, vector3f* C);
vector3f* vfTimesEq(vector3f* A, vector3f* B);
vector3f* vffMult(vector3f* A, vector3f* B, float mult);
vector3f* vffTimesEq(vector3f* A, float mult);

//divide element by element: C(i) = A(i)/B(i)
vector3f* vfDiv(vector3f* A, vector3f* B, vector3f* C);
vector3f* vfDivEq(vector3f* A, vector3f* B);
vector3f* vffDiv(vector3f* A, vector3f* B, float div);
vector3f* vffDivEq(vector3f* A, float div);

//calculate dot product A*B
float vfDot(vector3f* A, vector3f* B);

//Normalize A by its length
vector3f* vfNormalize(vector3f* A);

//Calculate the length of A
float vfLength(vector3f* A);

//Calculate the cross product: C=AxB
vector3f* vfCross(vector3f* C, vector3f* A, vector3f* B);

//Rotate 'target' about 'axis' through 'orgin' by theta
vector3f* vfAxisRotate(vector3f* target, vector3f* origin, vector3f* axis, double theta);

//Zero out the vector
vector3f* vfZero(vector3f* target);

#endif

