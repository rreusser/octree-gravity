#include "vector3f.h"

//ALLOCATE NEW VECTORS
//create a new float vector and return its pointer
vector3f* newVector3f(float x, float y, float z) {
    vector3f* newVec = malloc(sizeof(vector3f));
    newVec->x = x;
    newVec->y = y;
    newVec->z = z;
    return newVec;
}
//COPY VECTORS
//float: src --> target
vector3f* vfCopy(vector3f* target, vector3f* src) {
    target->x = src->x;
    target->y = src->y;
    target->z = src->z;
    return target;
}

//FLOAT VECTOR OPERATIONS
//Add two float vectors: A=B+C
vector3f* vfAdd(vector3f* A, vector3f* B, vector3f* C) {
    A->x = B->x+C->x;
    A->y = B->y+C->y;
    A->z = B->z+C->z;
    return A;
}
//A+=B
vector3f* vfPlusEq(vector3f* A, vector3f* B) {
    A->x += B->x;
    A->y += B->y;
    A->z += B->z;
    return A;
}

//subtract float vectors: A=B-C
vector3f* vfSub(vector3f* A, vector3f* B, vector3f* C) {
    A->x = B->x-C->x;
    A->y = B->y-C->y;
    A->z = B->z-C->z;
    return A;
}

//A-=B
vector3f* vfMinusEq(vector3f* A, vector3f* B) {
    A->x -= B->x;
    A->y -= B->y;
    A->z -= B->z;
    return A;
}

//calculate product element by element: A(i) = B(i)*C(i)
vector3f* vfMult(vector3f* A, vector3f* B, vector3f* C) {
    A->x = B->x*C->x;
    A->y = B->y*C->y;
    A->z = B->z*C->z;
    return A;
}

//A*=B
vector3f* vfTimesEq(vector3f* A, vector3f* B) {
    A->x *= B->x;
    A->y *= B->y;
    A->z *= B->z;
    return A;
}

//B=A*f
vector3f* vffMult(vector3f* A, vector3f* B, float mult) {
    A->x = B->x * mult;
    A->y = B->y * mult;
    A->z = B->z * mult;
    return A;
}

//A*=f
vector3f* vffTimesEq(vector3f* A, float mult) {
    A->x *= mult;
    A->y *= mult;
    A->z *= mult;
    return A;
}

//divide element by element: A(i) = B(i)/C(i)
vector3f* vfDiv(vector3f* A, vector3f* B, vector3f* C) {
    A->x = A->x/B->x;
    A->y = A->y/B->y;
    A->z = A->z/B->z;
    return A;
}
//A/=B
vector3f* vfDivEq(vector3f* A, vector3f* B) {
    A->x /= B->x;
    A->y /= B->y;
    A->z /= B->z;
    return A;
}

//B=A/f
vector3f* vffDiv(vector3f* A, vector3f* B, float div) {
    A->x = B->x / div;
    A->y = B->y / div;
    A->z = B->z / div;
    return A;
}

//A/=f
vector3f* vffDivEq(vector3f* A, float div) {
    A->x /= div;
    A->y /= div;
    A->z /= div;
    return A;
}


//calculate dot product A*B
float vfDot(vector3f* A, vector3f* B) {
    float sum;
    sum=A->x*B->x;
    sum+=A->y*B->y;
    sum+=A->z*B->z;
    return sum;
}

//Normalize A by its length
vector3f* vfNormalize(vector3f* A) {
    float len = vfLength(A);
    A->x /= len;
    A->y /= len;
    A->z /= len;
    return A;
}

//Calculate the length of A
float vfLength(vector3f* A) {
    float len = sqrt(A->x*A->x+A->y*A->y+A->z*A->z);
    return len;
}

//Calculate the cross product: A=BxC
vector3f* vfCross(vector3f* A, vector3f* B, vector3f* C) {
    A->x = B->y*C->z - B->z*C->y;
    A->y = B->z*C->x - B->x*C->z;
    A->z = B->x*C->y - B->y*C->x;
    return A;
}

//Rotate 'target' about 'axis' through 'orgin' by theta
vector3f* vfAxisRotate(vector3f* target, vector3f* origin, vector3f* axis, double theta) {
    double x = target->x;
    double y = target->y;
    double z = target->z;
    double a = origin->x;
    double b = origin->y;
    double c = origin->z;
    double u = axis->x;
    double v = axis->y;
    double w = axis->z;

    target->x = (a*(v*v+w*w)+u*(-b*v-c*w+u*x+v*y+w*z)+
	((x-a)*(v*v+w*w)+u*(b*v+c*w-v*y-w*z))*cos(theta)+
	sqrt(u*u+v*v+w*w)*(b*w-c*v-w*y+v*z)*sin(theta))/
	(u*u+v*v+w*w);
    target->y = (b*(u*u+w*w)+v*(-a*u-c*w+u*x+v*y+w*z)+
	((y-b)*(u*u+w*w)+v*(a*u+c*w-u*x-w*z))*cos(theta)+
	sqrt(u*u+v*v+w*w)*(-a*w+c*u+w*x-u*z)*sin(theta))/
	(u*u+v*v+w*w);
    target->z = (c*(u*u+v*v)+w*(-a*u-b*v+u*x+v*y+w*z)+
	((z-c)*(u*u+v*v)+w*(a*u+b*v-u*x-v*y))*cos(theta)+
	sqrt(u*u+v*v+w*w)*(a*v-b*u-v*x+u*y)*sin(theta))/
	(u*u+v*v+w*w);
    return target;
}

//Zero out the vector
vector3f* vfZero(vector3f* target) {
    target->x=target->y=target->z=0.0;
    return target;
}
