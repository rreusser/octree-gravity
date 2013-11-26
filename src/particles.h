#ifndef __PARTICLES_H__
#define __PARTICLES_H__


typedef struct {
    int n;
    real* x;             // state vector = x1, y1, z1, u1, v1, w1,  x2, y2, etc...
    real* m;             // particle mass
    real G;              // gravitational constant
    real dt,t;

    void (*gravity) (    // gravity function to calculate acceleration due to gravity - to be called by (*integrate)
	real, real*, int, real*, void* );

    void (*integrate) (  // function-pointer for RK
	real* y, void (*deriv_func)(real, real*, int, real*, void*), 
	real x, const int n, const real h, void* data );

} particles;

particles* alloc_particles( int n );

void free_particles( particles* p );

void randomize_particles( particles* p, real xmin, real xmax, real ymin, real ymax, real zmin, real zmax, real vel_scale );

void output_particles( FILE* f, particles* p );

void uniform_masses( particles* p, real mass );

void randomize_masses( particles* p, real mmin, real mmax );

void draw_particles( particles* p );

void write_particles( particles* p, const char* fname );

void bound_particles( particles* p, real range );

void read_particle_data( particles** p, const char* fname );

#endif /*__PARTICLES_H__*/
