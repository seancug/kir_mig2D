#ifndef KIR_h
#define KIR_H
#include <stdio.h>
#include <iostream>
#define PI 3.1415926535898

/* Numerical Recipes standard error handler */
void nrerror(char error_text[]);

/* allocate an int vector with subscript range v[nl..nh] */
int *ivector(long nl, long nh);

/* free an int vector allocated with ivector() */
void free_ivector(int *v, long nl, long nh);

/* allocate a float vector with subscript range v[nl..nh] */
float *vector(long nl, long nh);

/* free a float vector allocated with vector() */
void free_vector(float *v, long nl, long nh);

/* allocate a float matrix with subscript range m[nrl..nrh][ncl..nch] */
float **matrix(long nrl, long nrh, long ncl, long nch);

/* free a float matrix allocated by matrix() */
void free_matrix(float **m, long nrl, long nrh, long ncl, long nch);

/* initiate the parameters*/
int kir_init(float **arryin, float **arryvel, float *t, float *x, int *size, float *para);

/*read data from grd*/
float** rd_grd(char* filename, int &nt, int &nx, float &dt, float &dx);
#endif
