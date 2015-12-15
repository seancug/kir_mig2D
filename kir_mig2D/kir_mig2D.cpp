/** Copyright (C) 2015, Qingdao Institute of Marine Geology    **
**  All right reserved                                         **
**  2D Kirchhoff post time migtation                           **
**  Current Version:Debug 1.0                                  **
**  Author:Sun BO(Sean)                                        **
**  Finish Date:2015.12                                        **
**  For More Information,please contact sunbo@cgs.cn           ** 
**
**二维克西霍夫叠后时间偏移
**作者：孙波     青岛海洋地质研究所
**邮箱:sunbo@cgs.cn
**/

#include "stdafx.h"
#include "kir.h"


int main(int agrc, char *agrv[])
{
	float **arryin = NULL;   //输入叠加剖面
	float **arryvel = NULL;  //均方根速度
	float dt;         //时间间隔
	float dx;         //道间距
	float aper;       //偏移孔径，aperture
	float width_aper;      //孔径偏移cos边宽度
	float ang_lim;    //角度限制
	float width_ang;  //角度cos边
	int nx;     //地震道数
	int nt;     //采样点数
	float *x = NULL;
	float *t = NULL;
	float *para = NULL;
	int *size = NULL;
	para = vector(1, 6);
	size = ivector(1, 2);

	kir_init(arryin, arryvel, t, x, size, para);
	
	nt = size[1];	nx = size[2];	dt = para[1];	
	dx = para[2];	aper = para[3];	width_aper = para[4];	
	ang_lim = para[5];	width_ang = para[6];
	
	free_vector(para, 1, 6);
	free_ivector(size, 1, 2);

	system("pause");
    return 0;
}

