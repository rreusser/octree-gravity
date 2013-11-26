#ifndef __RK_H__
#define __RK_H__

void rk2(real* y, void (*deriv_func)(real, real*, int, real*, void*), real x, const int n, const real h, void* data);
void rk4(real* y, void (*deriv_func)(real, real*, int, real*, void*), real x, const int n, const real h, void* data);
void rk8(real* y, void (*deriv_func)(real, real*, int, real*, void*), real t, const int n, const real h, void* data);

#endif /*__RK_H__*/
