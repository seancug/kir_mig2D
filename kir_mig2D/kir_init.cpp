#include "stdafx.h"

#include "kir.h"

/*参数初始化
**********************************************
**参数：  
**    arryin:输入地震数据
**    arryvel:均方根速度
**    t:时间坐标
**    x:距离坐标
**********************************************
**    size[1]: nt,       size[2]: nx
**********************************************
**	  pata[1]: dt,       para[2]: dx
**	  para[3]: aper,     para[4]: width_aper
**	  para[5]: ang_lim,  para[6]: width_ang
**********************************************
*/
int kir_init(float ***aryin, float ***aryvel, float **t, float **x, int *size, float *para)
{
	float **seis = NULL, **vel = NULL;
	seis = rd_grd("../data/seisthrust.grd", size[1], size[2], para[1], para[2]);
	vel = rd_grd("../data/velrms.grd", size[1], size[2], para[1], para[2]);

	*aryin = seis;
	*aryvel = vel;
	para[3] = 3000.0;
	para[4] = 0.05*para[3];
	para[5] = PI / 3;
	para[6] = 0.15*para[5];
	float *tt,*xx;
	tt = vector(1, size[1]);
	xx = vector(1, size[2]);
	for (int j = 0; j < size[1]; j++) tt[j + 1] = j*para[1];
	for (int i = 0; i < size[2]; i++) xx[i + 1] = i*para[2];
	*t = tt;
	*x = xx;
	return 0;
}