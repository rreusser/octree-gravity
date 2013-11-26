#include <math.h>
#include "func.h"

//take an argument in degrees and return the radians equivalent
double radians(double deg) {
    return deg*M_PI/180.0;
}

//take an argument in radians and return the degree equivalent
double degrees(double rad) {
    return rad*180.0/M_PI;
}

//Absolute Value
double dabs(double A){ return (A<0.0)?(-A):(A); }

//Sign: pos=1, neg=-1
double dsgn(double A){ return (A<0.0)?(-1.0):(1.0); }

double dmax(double A,double B){ return (A>B)?(A):(B); }

double dmin(double A,double B){ return (A>B)?(B):(A); }
