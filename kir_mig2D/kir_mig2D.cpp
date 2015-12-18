/** Copyright (C) 2015, Qingdao Institute of Marine Geology    **
**  All right reserved                                         **
**  2D Kirchhoff post time migtation                           **
**  Current Version:Debug 1.0                                  **
**  Author:Sun BO(Sean)                                        **
**  Finish Date:2015.12                                        **
**  For More Information,please contact:                       **
**				                      sunbo@cgs.cn			   ** 
**
**二维克西霍夫叠后时间偏移
**作者：孙波     青岛海洋地质研究所
**邮箱:sunbo@cgs.cn
**/

#include "stdafx.h"
#include "kir.h"
#include <time.h>  

int main(int agrc, char *agrv[])
{
	//时间计算操作
	clock_t begin, bmig, nmig, end;
	double timeuse = 0.0;
	int hour = 0, min = 0, sec = 0;

	begin = clock();

	//得到程序开始计算时间
	time_t Curtime;
	time(&Curtime);
	tm *RetTime = localtime(&Curtime);
	char begin_time[250], end_time[250];
	sprintf(begin_time, "Begin Time: %04d-%02d-%02d %02d:%02d:%02d"
		, RetTime->tm_year + 1900
		, RetTime->tm_mon + 1
		, RetTime->tm_mday
		, RetTime->tm_hour
		, RetTime->tm_min
		, RetTime->tm_sec);
	printf("%s\n\n", begin_time);

	float **aryin = NULL;   //输入叠加剖面
	float **aryvel = NULL;  //均方根速度
	float ** arymig = NULL; //输出数据
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

	kir_init(&aryin, &aryvel, &t, &x, size, para);

	float aa = aryvel[1][1];
	nt = size[1];	nx = size[2];	dt = para[1];
	dx = para[2];	aper = para[3];	width_aper = para[4];
	ang_lim = para[5];	width_ang = para[6];

	free_vector(para, 1, 6);
	free_ivector(size, 1, 2);

	arymig = matrix(1, nx, 1, nt);
	int radius;
	radius = iround(0.5*aper / dx + width_aper / dx);

	int aper_f = 0, aper_e = 0;
	float t_radius = 0.0;
	float cosTheta = 0.0;
	float angle = ang_lim + width_ang;
	float *y2 = vector(1, nt);
	float tmp = 0.0;

	float dang = 1.0 / width_ang*PI*0.5;
	float daper = 1.0 / width_aper*PI*0.5;

	bmig = clock();
	unsigned short int p1 = 0;
	printf("\n");
	for (int ktr = 1; ktr <= nx; ktr++) {/*每一个地震道*/
		aper_f = max(1, ktr - radius);
		aper_e = min(nx, ktr + radius);

		for (int kt = 2; kt <= nt; kt++) {/*每个地震道上的采样点*/
			for (int kaper = aper_f; kaper <= aper_e; kaper++) {/*积分半径里面的地震道*/
				tmp = 0.0;
				t_radius = sqrt(pow(2.0*(kaper - ktr)*dx / aryvel[ktr][kt], 2.0) + pow(t[kt], 2.0));

//				cosTheta = t[kt] / t_radius;

				if (t_radius <= t[nt]) {
					spline(t, aryin[kaper], nt, 0, 0, y2);
					splint(t, aryin[kaper], y2, nt, t_radius, &tmp);
				}
				else {
					break;
				}

				if (abs(kaper - ktr)*dx > 0.5*aper) {
					tmp = tmp*cos(daper*(abs(kaper - ktr)*dx - 0.5*aper));
				}

				arymig[ktr][kt] = arymig[ktr][kt] + tmp;
				//				arymig[ktr][kt] = arymig[ktr][kt] + tmp;
			}
		//	arymig[ktr][kt] = arymig[ktr][kt] / (aryvel[ktr][kt] * 2 * PI);
		}

		/*nmig = clock();
		timeuse = (double)(nmig - bmig) / CLOCKS_PER_SEC;
		hour = (int)(timeuse / 60.0 / 60.0);
		min = (int)((timeuse - 60.0*60.0*hour) / 60.0);
		sec = (int)(timeuse - 60.0*60.0*hour - min * 60.0);
		*/
		static unsigned char w[] = "///-";
		if (p1++ == 3) p1 = 0;
	/*	printf("\rUsed Time::%dh %dm %ds\t--Finished Percent:%.1f %c %c",
			hour, min, sec, 100.0*(ktr) / nx, '%', w[p1]);*/
		printf("\r--Finished Percent:%.1f %c %c",
			100.0*(ktr) / nx, '%', w[p1]); 
		fflush(stdout);
	}

	free_vector(y2, 1, nt);
	free_matrix(aryin, 1, nx, 1, nt);
	free_matrix(aryvel, 1, nx, 1, nt);

	/*ouput data*/
	FILE *fpout = NULL;
	fpout = fopen("../data/data3.txt", "w");
	for (int j = 1; j <= nt; j++) {
		for (int i = 1; i <= nx; i++) {
			fprintf(fpout, "%.3e\t", arymig[i][j]);
		}
		fprintf(fpout, "\n");
	}
	fclose(fpout);

	free_matrix(arymig, 1, nx, 1, nt);
	free_vector(x, 1, nx);
	free_vector(t, 1, nt);

	end = clock();
	timeuse = (double)(end - begin) / CLOCKS_PER_SEC;
	hour = (int)(timeuse / 60.0 / 60.0);
	min = (int)((timeuse - 60.0*60.0*hour) / 60.0);
	sec = (int)(timeuse - 60.0*60.0*hour - min * 60.0);
	printf("\nCaculate time:%dh %dm %ds\n", hour, min, sec);

	time(&Curtime);
	tm *RetTime1 = localtime(&Curtime);
	sprintf(end_time, "End Time: %04d-%02d-%02d %02d:%02d:%02d \n"
		, RetTime1->tm_year + 1900
		, RetTime1->tm_mon + 1
		, RetTime1->tm_mday
		, RetTime1->tm_hour
		, RetTime1->tm_min
		, RetTime1->tm_sec);
	printf("%s\n", end_time);
	printf("Press the 'Enter' button to End\n");
	(void)getchar();
	return 0;
}

