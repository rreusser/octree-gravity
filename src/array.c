#include "array.h"

// Allocate a Liffe Array (pointer to a pointer ... to a pointer to a number)
// It ends up as just a regular void* with no special structure -- just a 
// shortcut for having to allocate manually

void* Alloc1DArray(const size_t size, const int num) {
    void* ptr = calloc(num,size);
    return ptr;
}

void Free1DArray(void* array) {
    if(array)
	free(array);
}

void** Alloc2DArray(const size_t size, const int ni, const int nj) {
    int i;
    void** ptr = malloc(ni*sizeof(void*));
    for(i=0;i<ni;i++) {
	ptr[i] = calloc(nj,size);
    }
    return ptr;
}

void Free2DArray(void** array, const int ni) {
    int i;
    for(i=0;i<ni;i++) {
	if(array[i])
	    free(array[i]);
    }
    if(array)
	free(array);
}


void*** Alloc3DArray(const size_t size, const int ni, const int nj, const int nk) {
    int i,j;
    void*** ptr = malloc(ni*sizeof(void**));
    for(i=0;i<ni;i++) {
	ptr[i] = malloc(nj*sizeof(void*));
    }
    for(i=0;i<ni;i++) {
	for(j=0;j<nj;j++) {
	    ptr[i][j] = calloc(nk,size);
	}
    }
    return ptr;
}

void Free3DArray(void*** array, const int ni, const int nj) {
    int i,j;
    for(i=0;i<ni;i++) {
	for(j=0;j<nj;j++) {
	    if(array[i][j])
		free(array[i][j]);
	}
    }
    for(i=0;i<ni;i++) {
	if(array[i])
	    free(array[i]);
    }
    if(array)
	free(array);
}


void**** Alloc4DArray(const size_t size, const int ni, const int nj, const int nk, const int nl) {
    int i,j,k;
    void**** ptr = malloc(ni*sizeof(void***));
    for(i=0;i<ni;i++) {
	ptr[i] = malloc(nj*sizeof(void**));
    }
    for(i=0;i<ni;i++) {
	for(j=0;j<nj;j++) {
	    ptr[i][j] = malloc(nk*sizeof(void*));
	}
    }
    for(i=0;i<ni;i++) {
	for(j=0;j<nj;j++) {
	    for(k=0;k<nk;k++) {
		ptr[i][j][k] = calloc(nl,size);
	    }
	}
    }
    return ptr;
}

void Free4DArray(void**** array, const int ni, const int nj, const int nk) {
    int i,j,k;
    for(i=0;i<ni;i++) {
	for(j=0;j<nj;j++) {
	    for(k=0;k<nk;k++) {
		if(array[i][j][k])
		    free(array[i][j][k]);
	    }
	}
    }
    for(i=0;i<ni;i++) {
	for(j=0;j<nj;j++) {
	    if(array[i][j])
		free(array[i][j]);
	}
    }
    for(i=0;i<ni;i++) {
	if(array[i])
	    free(array[i]);
    }
    if(array)
	free(array);
}


