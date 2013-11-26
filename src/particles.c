#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "common.h"
#include "particles.h"

particles* alloc_particles( int n ) {
    particles* p = malloc(sizeof(particles));
    p->n = n;
    p->x = malloc(sizeof(real)*n*6);
    p->m = malloc(sizeof(real)*n);
    p->t = 0.0;
    return p;
}

void free_particles( particles* p ) {
    if(p) {
	if(p->x) { free(p->x); }
	if(p->m) { free(p->m); }
	free(p);
    }
}

void read_particle_data( particles** p, const char* fname ) {
    int i,n;
    FILE* f;
    real din1,din2,din3,din4,din5,din6,din7;
    if( (f=fopen(fname,"r")) != NULL ) {
	fscanf(f,"%i",&n);
	fprintf(stderr,"Reading %i particles...",n);
	free_particles(*p);
	*p=alloc_particles( n );
	for(i=0;i<n;i++) {
	    fscanf(f,"%le %le %le %le %le %le %le",&din1,&din2,&din3,&din4,&din5,&din6,&din7);
	    (*p)->m[i] = din1;
	    (*p)->x[6*i  ] = din2 + drand48()*0.0001;
	    (*p)->x[6*i+1] = din4 + drand48()*0.0001;
	    (*p)->x[6*i+2] = din3 + drand48()*0.0001;
	    (*p)->x[6*i+3] = din5;
	    (*p)->x[6*i+4] = din7;
	    (*p)->x[6*i+5] = din6;
	}
	fclose(f);
	fprintf(stderr,"done.\n");
    } else {
	fprintf(stderr,"read_particle_data: Error reading `%s' for input.\n",fname);
    }
}


void randomize_particles( particles* p, real xmin, real xmax, real ymin, real ymax, real zmin, real zmax, real vel_scale ) {
    int i;
    srand48(21);
    real dx = xmax-xmin;
    real dy = ymax-ymin;
    real dz = zmax-zmin;
    for(i=0;i<p->n*6;i+=6) {
	p->x[i  ] = xmin + dx*drand48();
	p->x[i+1] = ymin + dy*drand48();
	p->x[i+2] = zmin + dz*drand48();
	p->x[i+3] = 2.0*(drand48()-0.5)*vel_scale;
	p->x[i+4] = 2.0*(drand48()-0.5)*vel_scale;
	p->x[i+5] = 2.0*(drand48()-0.5)*vel_scale;
    }
}

void output_particles( FILE* f, particles* p ) {
    int i,j;
    fprintf(f,"%7s%20s%20s%20s%20s%20s%20s%20s\n","#  ","x","y","z","u","v","w","m");
    for(i=0;i<p->n;i++) {
	fprintf(f,"%5i  ",i+1);
	for(j=0;j<6;j++) {
	    fprintf(f,"%20.15lf",p->x[6*i+j]);
	}
	fprintf(f,"%20.15lf",p->m[i]);
	fprintf(f,"\n");
    }
}

void uniform_masses( particles* p, real mass ) {
    int i;
    for(i=0;i<p->n;i++) {
	p->m[i] = mass;
    }
}

void randomize_masses( particles* p, real mmin, real mmax ) {
    int i;
    for(i=0;i<p->n;i++) {
	p->m[i] = mmin + drand48()*(mmax-mmin);
    }
}

void draw_particles( particles* p ) {
    int i;
    glPointSize(2.0);
    glBegin( GL_POINTS );
    for(i=0;i<p->n;i++) {
	glVertex3f( p->x[6*i], p->x[6*i+1], p->x[6*i+2] );
    }
    glEnd();
}

void write_particles( particles* p, const char* fname ) {
    int i;
    float fout;
    FILE* f;
    if( (f=fopen(fname, "w")) != NULL) {
	fwrite(&p->n,sizeof(int),1,f);
	for(i=0;i<p->n;i++) {
	    fout=(float)p->x[i*6];
	    fwrite(&fout,sizeof(float),1,f);
	    fout=(float)p->x[i*6+1];
	    fwrite(&fout,sizeof(float),1,f);
	    fout=(float)p->x[i*6+2];
	    fwrite(&fout,sizeof(float),1,f);
	}
	fclose(f);
    } else {
	fprintf(stderr,"Error opening file `%s' for writing.\n",fname);
    }
}

void bound_particles( particles* p, real range ) {
    int i,i6,j;
    for(i=0;i<p->n;i++) {
	i6 = i*6;
	for(j=0;j<3;j++) {
	    if(fabs(p->x[i6+j]) > range ) {
		p->x[i6+j] = SIGN(p->x[i6+j])*range;
	    }
	}
    }
}
