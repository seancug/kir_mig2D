#include "stdafx.h"

#include <stddef.h>
#include <stdlib.h>
#include "kir.h"
#define NR_END 1
#define FREE_ARG char*

void nrerror(char error_text[])
/*Standard error handler */
{
	fprintf(stderr, "Run-Time Error...\n");
	fprintf(stderr, "%s\n", error_text);
	fprintf(stderr, "...ow exiting to program...\n");
	exit(1);
}

int *ivector(long nl, long nh)
/* allocate an int vector with subscript range v[nl..nh] */
{
	int *v;
	long i;

	v = (int *)malloc((size_t)((nh - nl + 1 + NR_END)*sizeof(int)));
	if (!v) nrerror("allocation failure in ivector()");
	for (i = 0; i<(nh - nl + 1 + NR_END); i++) v[i] = 0;
	return v - nl + NR_END;
}

void free_ivector(int *v, long nl, long nh)
/* free an int vector allocated with ivector() */
{
	free((FREE_ARG)(v + nl - NR_END));
}

float *vector(long nl, long nh)
/* allocate a float vector with subscript range v[nl..nh] */
{
	float *v;
	long i;

	v = (float *)malloc((size_t)((nh - nl + 1 + NR_END)*sizeof(float)));
	if (!v) nrerror("Allocation Failure in Vector()");
	for (i = 0; i<(nh - nl + 1 + NR_END); i++) v[i] = 0.0;
	return v - nl + NR_END;
}

void free_vector(float *v, long nl, long nh)
/* free a float vector allocated with vector() */
{
	free((FREE_ARG)(v + nl - NR_END));
}

float **matrix(long nrl, long nrh, long ncl, long nch)
/* allocate a float matrix with subscript range m[nrl..nrh][ncl..nch] */
{
	long i, j, nrow = nrh - nrl + 1, ncol = nch - ncl + 1;
	float **m;

	/* allocate pointers to rows */
	m = (float **)malloc((size_t)((nrow + NR_END)*sizeof(float*)));
	if (!m) nrerror("Allocation Failure Row in Matrix()");
	m += NR_END;
	m -= nrl;

	/* allocate rows and set pointers to them */
	m[nrl] = (float *)malloc((size_t)((nrow*ncol + NR_END)*sizeof(float)));
	if (!m[nrl]) nrerror("Allocation Failure Column in Matrix()");
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for (i = nrl + 1; i <= nrh; i++) m[i] = m[i - 1] + ncol;

	/* initializing matrix */
	for (i = nrl; i <= nrh; i++)
		for (j = ncl; j <= nch; j++) m[i][j] = 0.0;
	/* return pointer to array of pointers to rows */
	return m;
}

void free_matrix(float **m, long nrl, long nrh, long ncl, long nch)
/* free a float matrix allocated by matrix() */
{
	free((FREE_ARG)(m[nrl] + ncl - NR_END));
	free((FREE_ARG)(m + nrl - NR_END));
}