#include "stdafx.h"

#include"kir.h"
#include<stdlib.h>

/*读入模型*/
float** rd_grd(char* filename, int &nt, int &nx, float &dt, float &dx)
{
	FILE *fp;
	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Open file \%s\ fails...\n", filename);
		return false;
	}

	int ret;		  /* fscanf() return value		 */
	unsigned long int vnum;   /* how many floats we have filled in the */
	float xmin = 0.0, xmax = 0.0, tmin = 0.0, tmax = 0.0, zmin = 0.0, zmax = 0.0;
	/*  current vector already */
	unsigned long int vec;    /* number of vector we are reading	*/

	printf("---Read the data from %s---\n", filename);
	char INDEX[6];
	fscanf(fp, "%s", INDEX);
	fscanf(fp, " %d %d ", &nx, &nt);
	fscanf(fp, " %f %f ", &xmin, &xmax);
	fscanf(fp, " %f %f ", &tmin, &tmax);
	fscanf(fp, " %f %f ", &zmin, &zmax);

	dx = fabs((xmax - xmin) / (nx - 1));
	dt = fabs((tmax - tmin) / (nt - 1));
	float **aryout = NULL;
	aryout = matrix(1, nx, 1, nt);//前后多开辟一个
	vnum = 1;		/* offset inside vector, in range 1->nx */
	vec = nt;		/* vector number, ends up as nt	 */

	unsigned short int p1 = 0;
	while (1) {
		ret = fscanf(fp, " %f ", &aryout[vnum][vec]);
		/*  fscanf returns: 0   : characters there, but no conversion (error)
		*		  EOF : eof before conversion
		*		  else: number of conversions
		*/
		if (ret == EOF) {
			if (vnum != 1)
				fprintf(stderr, "\n\tvector #%lu, float #%lu: we encountered an EOF but the vector has not been read in completely", vec + 1, vnum + 1);
			break;  /* else everything is okay: get out of the loop */
		}
		if (ret == 0)
			fprintf(stderr, "\n\tcould not read float in vector #%lu, float #%lu", vec + 1, vnum + 1);
		vnum++;
		if (vnum == (nx + 1)) {
			vnum = 1;
			static unsigned char w[] = "///-";
			if (p1++ == 3) p1 = 0;
			printf("\r\tread %s :%.1f %c %c", filename, 100.0*(nt - vec + 1) / nt, '%', w[p1]);
			fflush(stdout);
			vec--;
		}
	}
	printf("\n");
	fclose(fp);

	/*FILE *fpout = NULL;
	fpout = fopen("../data/aryin.txt", "w");
	for (int j = 1; j <= nt; j++) {
		for (int i = 1; i <= nx; i++) {
			fprintf(fpout, "%.3e\t", aryout[i][j]);
		}
		fprintf(fpout, "\n");
	}
	fclose(fpout);*/
	return aryout;
}
