#include <stdio.h>
#include <math.h>
#include <string.h>
#include "common.h"

void ArrayToGnuplot( const char* fname, real* x, real *y, const int n) {
    int i;
    FILE* output;
    if( (output=fopen(fname,"w")) != NULL) {
	for(i=0;i<n;i++) {
	    fprintf(output,"%e %e\n",x[i],y[i]);
	}
	fclose(output);
    } else {
	fprintf(stderr,"ArrayToGnuplot: Error opening file `%s' for writing.\n",fname);
    }
}

void GetArrayMinMax( real* a, const int n, real* min, real* max) {
    int i;
    *min = -log(0);
    *max = log(0);
    for(i=0;i<n;i++) {
	*min = fmin(a[i],*min);
	*max = fmax(a[i],*max);
    }
}

void GetIntArrayMinMax( int* a, const int n, int* min, int* max) {
    int i;
    *min = -log(0);
    *max = log(0);
    for(i=0;i<n;i++) {
	*min = MIN(a[i],*min);
	*max = MAX(a[i],*max);
    }
}

// Pointer debug
void dbgp( const char* s, void* ptr ) {
    fprintf(stderr,"%s = 0x%x\n",s,(unsigned int)ptr);
}

// Concatenate "prefix" + %(digits)i + "suffix" to dest
void create_filename(char* dest, const char* prefix, int i, int digits, const char* suffix ) {
    char fmt[10];
    int l;
    sprintf(fmt,"%%s%%%ii%%s",digits);
    sprintf(dest,fmt,prefix,i,suffix);
    l=strlen(dest);
    for(i=0;i<l;i++) {
	if(dest[i]==' ') {
	    dest[i]='0';
	}
    }
}

