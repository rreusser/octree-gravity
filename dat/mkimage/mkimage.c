#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <fftw3.h>
#include <string.h>
#include <math.h>

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))
#define ABS(a) ((a)>0?(a):-(a))
#define SQR(a) ((a)*(a))

int main( int argc, char** argv ) {
    FILE *fd;
    char fname[128];
    int n1, n2, frame, skip, c, i, j, k, n;
    double xmin, xmax, ymin, ymax, klen;
    unsigned char *im;
    float *input;
    double *fim, *x, pmax, *kernel, *kxbase, *kybase;
    fftw_complex *imh, *kh;
    fftw_plan forward_plan, inverse_plan, kernel_plan;
    int nx, ny, nxh, nyh;

    xmin = -1.0*0.6;
    xmax =  1.0*0.6;
    ymin = -1.0*0.6;
    ymax =  1.0*0.6;
    nx = 640;
    ny = 640;
    nxh = nx;
    nyh = ny/2+1;
    n1 = 0;
    n2 = 115;

    im = calloc(3*nx*ny,sizeof(unsigned char));
    fim = calloc(nx*ny,sizeof(*fim));
    imh = fftw_malloc(sizeof(fftw_complex)*nxh*nyh);
    //kernel = fftw_malloc(sizeof(*kernel)*nx*ny);
    kh = fftw_malloc(sizeof(fftw_complex)*nxh*nyh);
    kxbase = malloc(sizeof(double)*nxh);
    kybase = malloc(sizeof(double)*nyh);
    for(i=0; i<(nx+1)/2; i++)
	kxbase[i] = 1.0*i/nxh*nx;
    for(i=(nxh+1)/2; i<nxh; i++)
	kxbase[i] = -1.0*(nx-i)/nxh*nx;
    for(j=0; j<nyh; j++)
	kybase[j] = 1.0*j;

    forward_plan = fftw_plan_dft_r2c_2d(nx,ny,fim,imh,FFTW_ESTIMATE);
    inverse_plan = fftw_plan_dft_c2r_2d(nx,ny,imh,fim,FFTW_ESTIMATE);
    //kernel_plan = fftw_plan_dft_r2c_2d(nx,ny,kernel,kh,FFTW_ESTIMATE);

    //fftw_execute(kernel_plan);

    skip = 1;
    for(frame=n1, c=0; frame<n2; frame+=skip) {
	sprintf(fname,"../data_%6i.dat",frame);
	for(i=0; i<strlen(fname); i++) {
	    if(fname[i]==' ')
		fname[i]='0';
	}

	fprintf(stderr,"reading data from '%s'\n",fname);

	fd = fopen(fname,"r");
	fread(&n,sizeof(int),1,fd);
	n = 40000;
	input = malloc(sizeof(float)*n*3);
	x = malloc(sizeof(double)*n*3);
	fread(input,sizeof(float),n*3,fd);
	for(k=0;k<n*3;k++)
	    x[k]=input[k];
	fclose(fd);


	memset(fim,0,sizeof(double)*nx*ny);
	for(k=0; k<n; k++) {
	    i = (x[k*3]-xmin)/(xmax-xmin)*nx;
	    j = (x[k*3+2]-ymin)/(ymax-ymin)*ny;
	    //fprintf(stderr,"%f %f %i %i\n",x[k*3],x[k*3+1],i,j);
	    if(i>0 && j>0 && i<nx && j<ny) {
		fim[i*ny+j] += 1.0;
	    }
	}

	fftw_execute(forward_plan);
	double k1, k2;
	k1 = 0.0;
	k2 = 300.0;
	for(i=0; i<nxh; i++) {
	    for(j=0; j<nyh; j++) {
		klen = sqrt(SQR(kxbase[i])+SQR(kybase[j]));
		if (klen>k2) {
		    imh[i*nyh+j] *= 0;
		} else if (klen>k1) {
		    imh[i*nyh+j] *= 0.5+0.5*cos((klen-k1)/(k2-k1)*M_PI);
		}
		imh[i*nyh+j] /= nx*ny;
	    }
	}
	fftw_execute(inverse_plan);

	pmax = 0.3;
	for(k=0; k<nx*ny; k++) {
	    im[k*3  ] = MIN(MAX((int)((fim[k]/pmax)*255),0),255);
	    im[k*3+1] = MIN(MAX((int)((fim[k]/pmax)*255),0),255);
	    im[k*3+2] = MIN(MAX((int)((fim[k]/pmax)*255),0),255);
	    //fprintf(stderr,"%i %i %i\n",im[k*3],im[k*3+1],im[k*3+2]);
	}

	sprintf(fname,"../data_%6i.ppm",frame);
	for(i=0; i<strlen(fname); i++) {
	    if(fname[i]==' ')
		fname[i]='0';
	}
	fd = fopen(fname,"w");
	fprintf(fd,"P6\n");
	fprintf(fd,"# Barnes-hut ppm output\n");
	fprintf(fd,"%i %i\n%i\n",nx,ny,255);
	fwrite(im,sizeof(unsigned char),nx*ny*3,fd);
	fclose(fd);

	free(x);
	c++;
    }

    free(imh);
    free(im);
    free(fim);

    return 0;
}
