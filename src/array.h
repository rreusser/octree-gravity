#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <stdlib.h>
#include <stdio.h>

// Allocate a Liffe Array (pointer to a pointer ... to a pointer to a number)
// It ends up as just a regular void* with no special structure -- just a 
// shortcut for having to allocate manually

void* Alloc1DArray(const size_t size, const int num);
void Free1DArray(void* array);

void** Alloc2DArray(const size_t size, const int ni, const int nj);
void Free2DArray(void** array, const int ni);

void*** Alloc3DArray(const size_t size, const int ni, const int nj, const int nk);
void Free3DArray(void*** array, const int ni, const int nj);

void**** Alloc4DArray(const size_t size, const int ni, const int nj, const int nk, const int nl);
void Free4DArray(void**** array, const int ni, const int nj, const int nk);

#endif /*__ARRAY_H__*/
