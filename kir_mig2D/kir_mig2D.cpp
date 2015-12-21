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
#include <thread>
#include <vector>
#include <algorithm>

std::atomic<int> count(0);

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
	//float dt;         //时间间隔
	//float dx;         //道间距
	//float aper;       //偏移孔径，aperture
	float width_aper;      //孔径偏移cos边宽度
	float ang_lim;    //角度限制
	float width_ang;  //角度cos边
	//int nx;     //地震道数
	//int nt;     //采样点数
	float *x = NULL;
	float *t = NULL;
	float *para = NULL;
	int *size = NULL;
	para = vector(1, 6);
	size = ivector(1, 2);

	kir_init(&aryin, &aryvel, &t, &x, size, para);

	Params pd;
	pd.nt = size[1];	pd.nx = size[2];	pd.dt = para[1];
	pd.dx = para[2];	pd.aper = para[3];	
	pd.aryin = aryin;
	pd.aryvel = aryvel;
	pd.t = t;
	width_aper = para[4]; 
	ang_lim = para[5];	width_ang = para[6];

	free_vector(para, 1, 6);
	free_ivector(size, 1, 2);

	arymig = matrix(1, pd.nx, 1, pd.nt);
	pd.radius = iround(0.5*pd.aper / pd.dx + width_aper / pd.dx);
//	float angle = ang_lim + width_ang;
	pd.daper = 1.0 / width_aper*PI*0.5;

	int const nthread = 6;
	int const blocksize = pd.nx / nthread;
	std::vector<std::thread> threads(nthread - 1);
	int block_start = 1, block_end = 1;
	printf("\n");
	for (unsigned long i = 0; i < (nthread - 1); i++)
	{
		block_end = block_start;
		block_end += (blocksize - 1);
		printf("thread%d begin\n", i);
		threads[i] = std::thread(mig_loop, arymig, pd, block_start, block_end, i);
		block_start = block_end;
	}

	mig_loop(arymig, pd, block_start, (pd.nx + 1), -1);
	
	std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
	
	free_matrix(aryin, 1, pd.nx, 1, pd.nt);
	free_matrix(aryvel, 1, pd.nx, 1, pd.nt);

	/*ouput data*/
	FILE *fpout = NULL;
	fpout = fopen("../data/data_parallel.txt", "w");
	for (int j = 1; j <= pd.nt; j++) {
		for (int i = 1; i <= pd.nx; i++) {
			fprintf(fpout, "%.3e\t", arymig[i][j]);
		}
		fprintf(fpout, "\n");
	}
	fclose(fpout);

	free_matrix(arymig, 1, pd.nx, 1, pd.nt);
	free_vector(x, 1, pd.nx);
	free_vector(t, 1, pd.nt);

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

