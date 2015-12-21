#include "stdafx.h"

#include "kir.h"

void mig_loop(float **arymig, Params parin, int sp, int ep, int i)
{
	int aper_f = 0, aper_e = 0;
	float *y2 = vector(1, parin.nt);
	float t_radius = 0.0;
	float cosTheta = 0.0;
	float tmp = 0.0;
	extern std::atomic<int> count;
	unsigned short int p1 = 0;
	for (int ktr = sp; ktr < ep; ktr++) {/*每一个地震道*/
		aper_f = max(1, ktr - parin.radius);
		aper_e = min(parin.nx, ktr + parin.radius);

		for (int kt = 2; kt <= parin.nt; kt++) {/*每个地震道上的采样点*/
			for (int kaper = aper_f; kaper <= aper_e; kaper++) {/*积分半径里面的地震道*/
				tmp = 0.0;
				t_radius = sqrt(pow(2.0*(kaper - ktr)*parin.dx / parin.aryvel[ktr][kt], 2.0) + pow(parin.t[kt], 2.0));

				cosTheta = parin.t[kt] / t_radius;

				if (t_radius <= parin.t[parin.nt]) {
					spline(parin.t, parin.aryin[kaper], parin.nt, 0, 0, y2);
					splint(parin.t, parin.aryin[kaper], y2, parin.nt, t_radius, &tmp);
				}

				if (abs(kaper - ktr)*parin.dx > 0.5*parin.aper) {
					tmp = tmp*cos(parin.daper*(abs(kaper - ktr)*parin.dx - 0.5*parin.aper));
				}

				arymig[ktr][kt] = arymig[ktr][kt] + tmp*cosTheta / t_radius;
			}
			arymig[ktr][kt] = arymig[ktr][kt] / (parin.aryvel[ktr][kt] * parin.aryvel[ktr][kt] * 2 * PI);
		}
		count++;
		static unsigned char w[] = "///-";
		if (p1++ == 3) p1 = 0;
		printf("\r--Finished Percent:%.1f %c %c",
		100.0*(count) / parin.nx, '%', w[p1]);
		fflush(stdout);
	}
	printf("\nThread%d Complete\n", i);
	free_vector(y2, 1, parin.nt); 
}